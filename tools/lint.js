#!/usr/bin/env node
/**
 * tools/lint.js — 算法知识库内容质量门禁（零依赖，Node 14+）
 *
 * 用法：
 *   node tools/lint.js                 全量检查，打印分级报告
 *   node tools/lint.js --json          输出 JSON（供 Trae 智能体解析）
 *   node tools/lint.js --report        额外写出 tools/lint-report.md
 *   node tools/lint.js content/xxx.md  只检查指定文件
 *   node tools/lint.js --only=E011,E015  只跑指定规则
 *   node tools/lint.js --quiet         只打印汇总
 *
 * 退出码：0 = 无 error（warning 不阻塞）；1 = 存在 error；2 = 脚本自身异常
 *
 * 规则编号约定：
 *   E0xx = 结构性错误（必须修）；W0xx = 质量问题（建议修）
 */

'use strict';

const fs = require('fs');
const path = require('path');

const ROOT = path.resolve(__dirname, '..');
const CONTENT_DIR = path.join(ROOT, 'content');
const PROBLEMS_DIR = path.join(ROOT, 'problems');
const GRAPH_FILE = path.join(ROOT, 'knowledge_graph.json');
const INDEX_FILE = path.join(ROOT, 'index.html');
const DATA_FILE = path.join(ROOT, 'data.js');

const argv = process.argv.slice(2);
const JSON_OUT = argv.includes('--json');
const WRITE_REPORT = argv.includes('--report');
const QUIET = argv.includes('--quiet');
const onlyArg = argv.find(a => a.startsWith('--only='));
const ONLY = onlyArg ? new Set(onlyArg.slice(7).split(',').map(s => s.trim()).filter(Boolean)) : null;
const fileArgs = argv.filter(a => !a.startsWith('--'));

const findings = [];   // { level:'error'|'warn', code, file, line, msg, hint }
function add(level, code, file, line, msg, hint) {
  if (ONLY && !ONLY.has(code)) return;
  findings.push({ level, code, file, line: line || 1, msg, hint: hint || '' });
}
const err = (code, file, line, msg, hint) => add('error', code, file, line, msg, hint);
const warn = (code, file, line, msg, hint) => add('warn', code, file, line, msg, hint);

// ---------- 工具函数 ----------
function readText(p) {
  const buf = fs.readFileSync(p);
  let s = buf.toString('utf8');
  if (s.charCodeAt(0) === 0xfeff) s = s.slice(1); // 去 BOM
  return s;
}

function parseFrontmatter(text) {
  const m = text.match(/^---\r?\n([\s\S]*?)\r?\n---\r?\n?([\s\S]*)$/);
  if (!m) return { meta: {}, body: text, hasFrontmatter: false };
  const meta = {};
  m[1].split('\n').forEach(line => {
    const mm = line.match(/^(\w+):\s*(.*)$/);
    if (!mm) return;
    const key = mm[1];
    let val = mm[2].trim();
    if (val.startsWith('[') && val.endsWith(']')) {
      val = val.slice(1, -1).split(',').map(s => s.trim().replace(/^["']|["']$/g, '')).filter(Boolean);
    } else {
      val = val.replace(/^["']|["']$/g, '');
    }
    meta[key] = val;
  });
  return { meta, body: m[2], hasFrontmatter: true };
}

const lineOf = (text, index) => text.slice(0, index).split('\n').length;
const stripWs = s => s.replace(/\s+/g, '');

// 去掉代码块，得到"散文"部分（用于检查乱码、草稿表述、$ 配平）
function stripCodeFences(body) {
  return body.replace(/```[\s\S]*?```/g, m => '\n' + '\n'.repeat(m.split('\n').length - 2));
}
function extractFences(body) {
  const out = [];
  const re = /```([^\n]*)\n([\s\S]*?)```/g;
  let m;
  while ((m = re.exec(body))) out.push({ info: m[1], code: m[2], index: m.index });
  return out;
}
// 判断一个代码块是不是 C++（用于区分「代码」和「ASCII 图示 / 伪代码」）
function looksCpp(info, code) {
  return /cpp|c\+\+/i.test(info || '') || /#include|int main\s*\(/.test(code || '');
}
function cppBlocks(body) {
  return extractFences(body).filter(f => looksCpp(f.info, f.code));
}
// 取出一行中真正位于 $...$ 内部的片段（按配对切分，避免把「闭$ | 开$」误判成公式内竖线）
function mathSegments(line) {
  const parts = line.split(/(?<!\\)\$/);
  return parts.filter((_, i) => i % 2 === 1);
}
// 只保留"代码骨架"：去掉注释与所有空白，用于比较「正文代码块」与「.cpp」是否真的不同。
// 正文里常见的「带逐行中文注释的讲解版」与「.cpp 干净版」在去掉注释后应当完全一致。
function codeOnly(src) {
  return String(src)
    .replace(/\/\*[\s\S]*?\*\//g, ' ')   // 块注释
    .replace(/\/\/[^\n]*/g, ' ')         // 行注释
    .replace(/\s+/g, '');
}
function headings(body) {
  const out = [];
  body.replace(/^(#{1,6})[ \t]+(.+?)[ \t]*$/gm, (full, hashes, title) => {
    out.push({ level: hashes.length, title: title.replace(/#+\s*$/, '').trim() });
    return full;
  });
  return out;
}

// 正文常把完整代码拆成「连续若干块 + 每块后面跟一段讲解」的形式（逐行代码解析的常见写法）。
// 只要存在一段连续的代码块，把它们按顺序拼接后与 .cpp 的代码骨架一致，就说明正文与 .cpp 同步。
function hasContiguousCodeMatch(codes, fileNorm) {
  if (!fileNorm) return false;
  for (let a = 0; a < codes.length; a++) {
    let acc = '';
    for (let b = a; b < codes.length; b++) {
      acc += codes[b];
      if (acc === fileNorm) return true;
      if (acc.length >= fileNorm.length) break;
    }
  }
  return false;
}

// ---------- 规则数据 ----------
const DRAFT_PATTERNS = [
  [/(?:^|[。！？\n])\s*等等[，,。]/, '自我修正式表述「等等，」'],
  [/(?:^|[。！？\n])\s*不对[，,。]/, '自我修正式表述「不对，」'],
  [/(?:^|[。！？\n])\s*纠正一下/, '自我修正式表述「纠正一下」'],
  [/\？\s*不[，,]/, '草稿残留「？不，」'],
  [/\?\s*不[，,]/, '草稿残留「?不，」'],
  [/我再想想/, '草稿残留「我再想想」'],
  // 「占位」在算法讲解里是正常技术术语（下标 0 占位 / 哨兵占位），不是未完成标记；
  // 这里只匹配明确的待办措辞，避免把「ans[0] 占位」这类写法误报成占位内容。
  [/待补充|待完善|占位符|TODO|FIXME/, '未完成的占位内容'],
];
const TITLE_DRAFT_PATTERNS = [
  [/自己写的/, '标题含草稿式自述「自己写的」'],
  [/以前没有学/, '标题含草稿式自述「以前没有学」'],
  [/未完成|半成品/, '标题含「未完成」'],
  [/^[A-Z]\d?\.\s/, '标题以题库内部编号开头（如 "D. xxx"）'],
];
const MOJIBAKE = [
  ['代紅', '乱码「代紅」（应为「洛谷」）'],
  ['鈥', 'UTF-8 被按 GBK 解码产生的乱码'],
  ['锟斤拷', '典型乱码「锟斤拷」'],
  ['\uFFFD', '替换字符 U+FFFD（编码损坏）'],
];
const DIFFICULTY_OK = ['入门', '简单', '中等', '困难', '提高', '普及'];
const KP_SECTIONS_REQUIRED = ['算法原理', '逐行代码解析', '复杂度分析', '常见陷阱'];
const PROB_SECTIONS_REQUIRED = ['题目描述', '样例', '解题思路', '复杂度分析', '常见陷阱'];

// ---------- 收集文件 ----------
let contentFiles = [], problemFiles = [];
try { contentFiles = fs.readdirSync(CONTENT_DIR).filter(f => f.endsWith('.md')).sort(); } catch (e) {
  console.error('❌ 无法读取 content/ 目录：' + e.message); process.exit(2);
}
try { problemFiles = fs.readdirSync(PROBLEMS_DIR).filter(f => f.endsWith('.md')).sort(); } catch (e) { problemFiles = []; }

if (fileArgs.length) {
  const wanted = new Set(fileArgs.map(f => path.basename(f)));
  contentFiles = contentFiles.filter(f => wanted.has(f));
  problemFiles = problemFiles.filter(f => wanted.has(f));
}

let graph = { categories: {}, subcategories: {}, knowledgePoints: {}, subSubcategories: {} };
if (fs.existsSync(GRAPH_FILE)) {
  try { graph = Object.assign(graph, JSON.parse(readText(GRAPH_FILE))); }
  catch (e) { err('E900', 'knowledge_graph.json', 1, 'JSON 解析失败：' + e.message); }
}
const graphKP = graph.knowledgePoints || {};

// ---------- 主检查 ----------
const seenIds = new Map();      // id -> file
const seenCodePath = new Map(); // codePath -> [files]
const knownIds = new Set();
const records = [];
const codeSync = { exact: 0, annotated: 0, drift: 0, fragment: 0 };

// 预填充「已知 id」：即使本次只检查若干文件，跨文件引用校验（E011/W032）也要以全量为准，
// 否则单文件检查会把所有 prerequisites/related 误报成"指向不存在"。
try {
  fs.readdirSync(CONTENT_DIR).filter(f => f.endsWith('.md')).forEach(f => knownIds.add(path.basename(f, '.md')));
} catch (e) { /* 已在上方处理 */ }
try {
  fs.readdirSync(PROBLEMS_DIR).filter(f => f.endsWith('.md')).forEach(f => knownIds.add(path.basename(f, '.md')));
} catch (e) { /* problems/ 可能不存在 */ }

function checkFile(kind, filename) {
  const rel = (kind === 'kp' ? 'content/' : 'problems/') + filename;
  const full = path.join(kind === 'kp' ? CONTENT_DIR : PROBLEMS_DIR, filename);
  const text = readText(full);
  const { meta, body, hasFrontmatter } = parseFrontmatter(text);

  if (!hasFrontmatter) {
    err('E001', rel, 1, '缺少 frontmatter（文件必须以 --- 开头的 YAML 块开始）');
    return null;
  }

  // --- 必填字段 ---
  const required = kind === 'kp' ? ['id', 'title', 'category', 'subcategory'] : ['id', 'title', 'difficulty', 'category'];
  required.forEach(f => {
    if (!meta[f]) err('E002', rel, 2, `frontmatter 缺少必填字段 "${f}"`);
  });

  // --- id 规范 ---
  const id = meta.id || path.basename(filename, '.md');
  const expectId = path.basename(filename, '.md');
  if (meta.id && meta.id !== expectId) {
    err('E003', rel, 2, `id (${meta.id}) 与文件名 (${expectId}) 不一致`, 'id 必须等于文件名（不含 .md），否则锚点/互链会失效');
  }
  if (!/^[a-z0-9][a-z0-9-]*$/.test(id)) {
    warn('W001', rel, 2, `id "${id}" 不符合命名规范（小写字母/数字/连字符）`);
  }
  if (seenIds.has(id)) {
    err('E004', rel, 2, `id "${id}" 重复（已在 ${seenIds.get(id)} 出现）`);
  } else {
    seenIds.set(id, rel);
  }
  knownIds.add(id);

  // --- 标题草稿化 ---
  const title = meta.title || '';
  if (title) {
    TITLE_DRAFT_PATTERNS.forEach(([re, msg]) => {
      if (re.test(title)) warn('W002', rel, 3, `${msg}：${title}`, '请改成正式的算法/题目名称');
    });
    if (/[（(]\s*[）)]/.test(title)) warn('W003', rel, 3, '标题含空的括号');
  }

  // --- 分类 ---
  if (kind === 'kp') {
    if (meta.category && graph.categories && Object.keys(graph.categories).length &&
        !graph.categories[meta.category]) {
      warn('W004', rel, 3, `category "${meta.category}" 未在 knowledge_graph.json 的 categories 中登记`);
    }
    if (meta.category && meta.subcategory) {
      const key = `${meta.category}|${meta.subcategory}`;
      if (graph.subcategories && Object.keys(graph.subcategories).length && !graph.subcategories[key]) {
        warn('W005', rel, 4, `subcategory "${key}" 未在 knowledge_graph.json 的 subcategories 中登记`);
      }
    }
    if (meta.subSubcategory) {
      const key = `${meta.category}|${meta.subcategory}|${meta.subSubcategory}`;
      if (graph.subSubcategories && Object.keys(graph.subSubcategories).length && !graph.subSubcategories[key]) {
        warn('W006', rel, 5, `subSubcategory "${key}" 未在 subSubcategories 中登记`);
      }
    }
  }

  // --- 复杂度字段 ---
  if (kind === 'kp' && meta.category !== '初赛笔记') {
    ['timeComplexity', 'spaceComplexity'].forEach(f => {
      if (!meta[f]) warn('W007', rel, 5, `缺少 ${f}`);
      else if (/未知|unknown|待定/i.test(String(meta[f]))) warn('W008', rel, 5, `${f} 为"未知"，与正文复杂度分析矛盾`, '回填真实复杂度，它决定该条目能否被检索到');
    });
  }

  // --- 标签 ---
  const tags = Array.isArray(meta.tags) ? meta.tags : (meta.tags ? [meta.tags] : []);
  if (!tags.length) warn('W009', rel, 6, '没有任何标签');
  if (tags.length > 8) warn('W010', rel, 6, `标签过多（${tags.length} 个），会稀释检索价值`, '建议每题 3–6 个');
  const dupTags = tags.filter((t, i) => tags.indexOf(t) !== i);
  if (dupTags.length) err('E005', rel, 6, `标签重复：${[...new Set(dupTags)].join('、')}`);
  if (tags.some(t => !t || !String(t).trim())) err('E006', rel, 6, '存在空标签');

  // --- codePath ---
  const codePath = meta.codePath || '';
  let codeRealPath = null;
  if (codePath) {
    codeRealPath = path.join(ROOT, codePath.replace(/[\\/]/g, path.sep));
    if (!fs.existsSync(codeRealPath)) {
      err('E007', rel, 6, `codePath 指向的文件不存在：${codePath}`);
      codeRealPath = null;
    }
    const prev = seenCodePath.get(codePath) || [];
    prev.push(rel);
    seenCodePath.set(codePath, prev);
    const ext = path.extname(codePath).toLowerCase();
    if (ext !== '.cpp') warn('W011', rel, 6, `codePath 扩展名不是 .cpp（${ext}）`);
  } else if (kind === 'kp' && meta.category !== '初赛笔记') {
    warn('W012', rel, 6, '没有 codePath（非初赛笔记类知识点通常应附带代码）');
  }

  // --- 知识图谱关系 ---
  if (kind === 'kp') {
    const entry = graphKP[id];
    if (!entry) {
      warn('W013', rel, 2, 'knowledge_graph.json 中没有该知识点的 prerequisites/related 条目');
    } else {
      ['prerequisites', 'related'].forEach(field => {
        const arr = entry[field];
        if (arr === undefined) return;
        if (!Array.isArray(arr)) { err('E008', rel, 2, `knowledge_graph.json 中 ${id}.${field} 不是数组`); return; }
        arr.forEach(target => {
          if (target === id) err('E008', rel, 2, `${field} 指向自己（${id}）`);
          else if (!/^[a-z0-9][a-z0-9-]*$/.test(target)) warn('W014', rel, 2, `${field} 中的 "${target}" 不像合法 id`);
        });
      });
    }
  }

  // --- 正文检查 ---
  const prose = stripCodeFences(body);
  const lines = text.split('\n');

  // 行尾：孤立 CR / BOM
  const loneCR = (text.match(/\r(?!\n)/g) || []).length;
  if (loneCR) warn('W015', rel, 1, `存在 ${loneCR} 个孤立 CR 字符（\\r 后不跟 \\n）`, '按 CommonMark 孤立 CR 也是换行符，会让代码块每行多出空行；统一为 LF 或 CRLF');
  if (/\r\r\n/.test(text)) err('E009', rel, 1, '存在 CR CR LF 行尾（\\r\\r\\n）', '多半是编码转换残留，会让渲染多出空行');
  if (text.charCodeAt(0) === 0xfeff) warn('W016', rel, 1, '文件以 BOM 开头');

  // 乱码
  MOJIBAKE.forEach(([pat, msg]) => {
    const idx = text.indexOf(pat);
    if (idx >= 0) {
      err('E010', rel, lineOf(text, idx), `${msg}`);
    }
  });

  // 草稿式表述（只在散文里查，代码块里的注释另行提示）
  DRAFT_PATTERNS.forEach(([re, msg]) => {
    const m = prose.match(re);
    if (m) warn('W017', rel, lineOf(prose, prose.indexOf(m[0])), `${msg}：…${m[0]}…`, '正式内容不应保留推导过程中的自我修正');
  });

  // $ 配平（散文部分）
  const dollarCount = (prose.match(/(?<!\\)\$/g) || []).length;
  if (dollarCount % 2 !== 0) {
    warn('W018', rel, 1, `行内公式 $ 数量为奇数（${dollarCount} 个），可能有未闭合的公式`);
  }

  // 表格内未转义的竖线（仅统计真正位于 $...$ 内部的竖线）
  prose.split('\n').forEach((ln, i) => {
    if (!ln.trim().startsWith('|')) return;
    const bad = mathSegments(ln).filter(s => /(?<!\\)\|/.test(s));
    if (bad.length) {
      warn('W019', rel, i + 1, `表格中的数学公式存在未转义竖线：$${bad[0].slice(0, 30)}$`, '写成 \\| 否则表格会被拆成多余单元格');
    }
  });

  // 标题层级
  const hs = headings(body);
  hs.forEach(h => {
    if (h.level === 1) warn('W020', rel, lineOf(body, body.indexOf('#' + ' ' + h.title)), `正文出现一级标题「${h.title}」`, '条目标题已由 frontmatter 提供，正文应从 ## 开始');
  });
  const titleSeen = new Map();
  hs.filter(h => h.level === 2).forEach(h => {
    const key = h.title.replace(/\s+/g, '');
    if (titleSeen.has(key)) warn('W021', rel, 1, `重复的二级标题「${h.title}」`, '常见于「## 适用场景」后紧跟「### 适用场景」');
    titleSeen.set(key, true);
  });

  // 章节完整性
  const h2 = hs.filter(h => h.level === 2).map(h => h.title);
  const needs = kind === 'kp'
    ? (meta.category === '初赛笔记' ? [] : KP_SECTIONS_REQUIRED)
    : PROB_SECTIONS_REQUIRED;
  needs.forEach(sec => {
    if (!h2.some(t => t.includes(sec))) {
      warn('W022', rel, 1, `缺少必备章节「${sec}」（现有：${h2.slice(0, 8).join(' / ')}）`);
    }
  });
  if (kind === 'kp' && meta.category !== '初赛笔记' && !h2.some(t => t.includes('适用场景'))) {
    warn('W023', rel, 1, '缺少「适用场景」章节');
  }
  if (kind === 'prob') {
    ['输入格式', '输出格式', '数据范围'].forEach(sec => {
      if (!body.includes(sec)) warn('W024', rel, 1, `题目正文缺少「${sec}」小节`);
    });
    if (!/[Mm]arkdown|```/.test(body) && !/样例/.test(body)) warn('W025', rel, 1, '题目正文似乎没有样例');
    if (meta.difficulty && !DIFFICULTY_OK.includes(String(meta.difficulty))) {
      warn('W026', rel, 3, `难度 "${meta.difficulty}" 不在约定枚举（${DIFFICULTY_OK.join('/')}）中`);
    }
    ['oj', 'problemId'].forEach(f => {
      if (!meta[f]) warn('W027', rel, 3, `缺少 ${f}（读者无法找到原题）`);
    });
  }

  // 内嵌代码 vs 实际 .cpp 文件
  if (codeRealPath) {
    const fileNorm = codeOnly(readText(codeRealPath));
    const raw = cppBlocks(body).map(f => f.code);
    const codes = raw.map(codeOnly);
    if (!codes.length) {
      warn('W029', rel, 1, '正文没有任何 C++ 代码块', '「逐行代码解析」至少要给出可读的代码片段或完整代码（完整代码可选：页面会自动渲染 .cpp）');
      codeSync.fragment++;
    } else if (codes.some(b => b === fileNorm)) {
      // 代码骨架一致：正文可能是「带注释的讲解版」，这是合格写法
      codeSync.exact++;
      const annotated = raw.some(b => stripWs(b) !== stripWs(readText(codeRealPath))) ;
      if (annotated) codeSync.annotated++;
    } else if (hasContiguousCodeMatch(codes, fileNorm)) {
      // 正文把完整代码拆成连续若干块来逐块讲解，拼接后与 .cpp 骨架一致，同样算同步
      codeSync.exact++;
      codeSync.annotated++;
    } else {
      const maxLen = codes.reduce((a, b) => Math.max(a, b.length), 0);
      const ratio = fileNorm.length ? maxLen / fileNorm.length : 0;
      if (ratio >= 0.6) {
        warn('W028', rel, 1,
          `正文中的整段代码与实际 .cpp 的**代码本身**不一致（剔除注释后仍不同，最长代码块为文件的 ${(ratio * 100).toFixed(0)}%）`,
          '正文与 .cpp 至少有一边改过而另一边没跟上；以 .cpp 为准同步，或确认两者本就是不同实现并写明区别');
        codeSync.drift++;
      } else {
        codeSync.fragment++; // 片段式讲解，可接受
      }
    }
  }

  // 代码块内的调试残留（乱码由上面的 MOJIBAKE 检查统一负责，避免重复计数）
  extractFences(body).forEach(f => {
    if (/(printf|cout)\s*<<?\s*"[^"]*(耗时|计时|调试|debug|test)/i.test(f.code)) {
      warn('W030', rel, lineOf(body, f.index), '代码块疑似含调试/计时输出', '提交 OJ 会因多余输出 WA');
    }
  });

  records.push({ kind, rel, id, meta, body, codePath, tags });
  return { rel, meta, body };
}

contentFiles.forEach(f => checkFile('kp', f));
problemFiles.forEach(f => checkFile('prob', f));

// ---------- 跨文件检查 ----------
// 同一 codePath 被多处引用
seenCodePath.forEach((files, cp) => {
  if (files.length > 1) {
    warn('W031', files[0], 6, `codePath 被 ${files.length} 个条目共用：${files.join('、')}`, `若确实是同一份模板可忽略：${cp}`);
  }
});

// 图谱指向不存在的知识点
let graphOrphanTargets = 0;
Object.keys(graphKP).forEach(id => {
  const entry = graphKP[id] || {};
  ['prerequisites', 'related'].forEach(field => {
    (Array.isArray(entry[field]) ? entry[field] : []).forEach(t => {
      if (!knownIds.has(t)) {
        graphOrphanTargets++;
        err('E011', 'knowledge_graph.json', 1, `${id}.${field} 指向不存在的知识点 "${t}"`);
      }
    });
  });
});

// 图谱里有、content 里没有
let graphOrphan = 0;
Object.keys(graphKP).forEach(id => {
  if (!knownIds.has(id)) {
    graphOrphan++;
    warn('W032', 'knowledge_graph.json', 1, `图谱中的 "${id}" 没有对应的 content/*.md`);
  }
});

// related 不对称（仅提示）
let asym = 0;
Object.keys(graphKP).forEach(id => {
  const rel = (graphKP[id] || {}).related;
  if (!Array.isArray(rel)) return;
  rel.forEach(t => {
    const back = (graphKP[t] || {}).related;
    if (Array.isArray(back) && !back.includes(id)) asym++;
  });
});
if (asym) warn('W033', 'knowledge_graph.json', 1, `related 关系有 ${asym} 处不对称（A 指向 B，B 未指向 A）`);

// ---------- 构建产物新鲜度 ----------
if (fs.existsSync(DATA_FILE) && !fileArgs.length) {
  const dataMtime = fs.statSync(DATA_FILE).mtimeMs;
  const newest = contentFiles.concat(problemFiles).reduce((acc, f) => {
    const dir = contentFiles.includes(f) ? CONTENT_DIR : PROBLEMS_DIR;
    return Math.max(acc, fs.statSync(path.join(dir, f)).mtimeMs);
  }, 0);
  if (newest > dataMtime) {
    warn('W034', 'data.js', 1, 'content/ 或 problems/ 比 data.js 更新 → 构建产物可能已过期',
      '运行 node build.js 重新生成。注意：本规则只比 mtime，"文件被碰过但内容没变"也会触发；' +
      '先确认是否真的改过内容（git status / git diff），再决定要不要重新构建');
  }
}

// ---------- index.html 预渲染目录一致性 ----------
if (fs.existsSync(INDEX_FILE) && !fileArgs.length) {
  const html = readText(INDEX_FILE);
  // 只在"静态标记"里统计：JS 模板字符串里也会出现这些 class，必须先剥掉 <script> 内容
  const htmlStatic = html.replace(/<script[\s\S]*?<\/script>/gi, '');
  const catalogCount = (htmlStatic.match(/class="catalog-kp-item"/g) || []).length;
  const sidebarCount = (htmlStatic.match(/class="kp-item[^"]*"/g) || []).length;
  const footerMatch = html.match(/<span id="kp-count">(\d+)<\/span>/);
  if (catalogCount && catalogCount !== contentFiles.length) {
    warn('W035', 'index.html', 1,
      `index.html 内预渲染目录有 ${catalogCount} 条，content/ 有 ${contentFiles.length} 条`,
      '预渲染块由手工维护、容易过期；建议改为构建时生成或直接删除该块');
  }
  if (footerMatch && Number(footerMatch[1]) !== contentFiles.length) {
    warn('W036', 'index.html', 1, `index.html 侧栏写死 "${footerMatch[1]} 个知识点"，实际 ${contentFiles.length} 条`);
  }
  if (sidebarCount && sidebarCount !== contentFiles.length) {
    warn('W037', 'index.html', 1, `index.html 内预渲染侧栏有 ${sidebarCount} 条，content/ 有 ${contentFiles.length} 条`);
  }
  if (!/<meta\s+name="description"/i.test(html)) warn('W038', 'index.html', 1, '缺少 <meta name="description">');
  if (!/rel="icon"/i.test(html)) warn('W039', 'index.html', 1, '缺少 favicon（<link rel="icon">）');
  if (!/property="og:/i.test(html)) warn('W040', 'index.html', 1, '缺少 Open Graph 标签（分享预览会没内容）');

  // W041/W042：JS 引用的 DOM 元素在文件里是否存在（防止「改了 HTML 导致页面空白」）
  // 统计范围包含 JS 模板字符串（例如 id="code-block" 是运行时拼出来的）
  const idsDefined = new Set([...html.matchAll(/\bid=["']([\w-]+)["']/g)].map(m => m[1]));
  const idsUsed = new Set([...html.matchAll(/getElementById\(\s*["']([\w-]+)["']\s*\)/g)].map(m => m[1]));
  const missingIds = [...idsUsed].filter(id => !idsDefined.has(id));
  if (missingIds.length) {
    warn('W041', 'index.html', 1,
      `JS 里 getElementById 引用了不存在的 id：${missingIds.join('、')}`,
      '多半是删/改了 HTML 结构而没同步 JS，会导致运行时报错、页面空白');
  }
  const classesUsed = new Set([...html.matchAll(/querySelector(?:All)?\(\s*["']\.([\w-]+)["']/g)].map(m => m[1]));
  const missingCls = [...classesUsed].filter(c => !new RegExp('class=["\'][^"\']*\\b' + c + '\\b').test(html));
  if (missingCls.length) {
    warn('W042', 'index.html', 1, `querySelector 引用了文件中不存在的 class：${missingCls.join('、')}`);
  }
}

// ---------- 输出 ----------
const errors = findings.filter(f => f.level === 'error');
const warns = findings.filter(f => f.level === 'warn');

const byCode = {};
findings.forEach(f => { (byCode[f.code] = byCode[f.code] || { level: f.level, n: 0, sample: f }); byCode[f.code].n++; });
const byFile = {};
findings.forEach(f => { byFile[f.file] = (byFile[f.file] || 0) + 1; });

if (JSON_OUT) {
  console.log(JSON.stringify({
    summary: {
      files: contentFiles.length + problemFiles.length,
      knowledgePoints: contentFiles.length,
      problems: problemFiles.length,
      errors: errors.length, warnings: warns.length,
    },
    byCode: Object.keys(byCode).sort().map(c => ({ code: c, level: byCode[c].level, count: byCode[c].n, sample: byCode[c].sample.msg })),
    byFile,
    findings,
  }, null, 2));
} else {
  const groupBy = list => {
    const g = {};
    list.forEach(f => { (g[f.file] = g[f.file] || []).push(f); });
    return g;
  };
  if (!QUIET) {
    if (errors.length) {
      console.log('\n' + '='.repeat(72));
      console.log(`❌ 错误 ${errors.length} 条（必须修）`);
      console.log('='.repeat(72));
      const g = groupBy(errors);
      Object.keys(g).sort().forEach(file => {
        console.log(`\n📄 ${file}`);
        g[file].forEach(f => {
          console.log(`   [${f.code}] L${f.line} ${f.msg}`);
          if (f.hint) console.log(`          ↳ ${f.hint}`);
        });
      });
    }
    if (warns.length) {
      console.log('\n' + '='.repeat(72));
      console.log(`⚠️  警告 ${warns.length} 条（建议修）`);
      console.log('='.repeat(72));
      // 警告按规则聚合展示，避免刷屏
      const byCodeWarn = {};
      warns.forEach(f => { (byCodeWarn[f.code] = byCodeWarn[f.code] || []).push(f); });
      Object.keys(byCodeWarn).sort().forEach(code => {
        const list = byCodeWarn[code];
        console.log(`\n[${code}] ${list.length} 处 — ${list[0].msg}`);
        if (list[0].hint) console.log(`   ↳ ${list[0].hint}`);
        list.slice(0, 12).forEach(f => console.log(`   · ${f.file}${f.line ? ':' + f.line : ''}`));
        if (list.length > 12) console.log(`   · …还有 ${list.length - 12} 处`);
      });
    }
  }
  console.log('\n' + '-'.repeat(72));
  console.log(`📊 知识点 ${contentFiles.length} 篇 · 题目 ${problemFiles.length} 道 · 错误 ${errors.length} · 警告 ${warns.length}`);
  console.log(`🔗 正文代码与 .cpp 同步情况：代码一致 ${codeSync.exact}（其中带注释讲解版 ${codeSync.annotated}）· 真漂移 ${codeSync.drift} · 片段式 ${codeSync.fragment}`);
  console.log(errors.length ? '❌ 存在错误，请修复后再构建/发布' : (warns.length ? '✅ 无阻塞错误（仍有警告待清理）' : '✅ 全部通过'));
  console.log('-'.repeat(72));
}

if (WRITE_REPORT) {
  const out = ['# 内容质量检查报告', '', `生成时间：${new Date().toLocaleString('zh-CN')}`, '',
    `- 知识点：${contentFiles.length} 篇`, `- 题目：${problemFiles.length} 道`,
    `- 错误：${errors.length} 条`, `- 警告：${warns.length} 条`, '',
    '## 按规则统计', '', '| 规则 | 级别 | 数量 | 说明 |', '|---|---|---|---|'];
  Object.keys(byCode).sort().forEach(c => {
    out.push(`| ${c} | ${byCode[c].level === 'error' ? '错误' : '警告'} | ${byCode[c].n} | ${byCode[c].sample.msg.replace(/\|/g, '\\|')} |`);
  });
  out.push('', '## 明细', '');
  findings.forEach(f => out.push(`- [${f.level === 'error' ? 'x' : ' '}] \`${f.code}\` **${f.file}**:${f.line} — ${f.msg}`));
  fs.writeFileSync(path.join(ROOT, 'tools', 'lint-report.md'), out.join('\n'), 'utf8');
  if (!JSON_OUT) console.log('📝 报告已写入 tools/lint-report.md');
}

process.exit(errors.length ? 1 : 0);
