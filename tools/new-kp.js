#!/usr/bin/env node
/**
 * tools/new-kp.js — 知识点脚手架
 *
 * 生成：content/<id>.md（按 templates/kp-template.md）
 *      代码文件（**只放任务卡注释，不含任何实现** —— 实现由用户本人写）
 *      并在 knowledge_graph.json 注册 prerequisites/related 空条目
 *
 * 用法：
 *   node tools/new-kp.js --id dp-interval-stone --title "区间DP·石子合并" \
 *        --category 动态规划 --subcategory 区间DP \
 *        [--subSubcategory ""] [--tags "动态规划,区间DP,DP"] \
 *        [--time "O(n^3)"] [--space "O(n^2)"] [--level B] \
 *        [--code "动态规划\区间DP\石子合并\Untitled1.cpp"] [--no-graph] [--force] [--dry-run]
 */

'use strict';
const fs = require('fs');
const path = require('path');

const ROOT = path.resolve(__dirname, '..');
const CONTENT = path.join(ROOT, 'content');
const GRAPH = path.join(ROOT, 'knowledge_graph.json');
const TPL = path.join(ROOT, 'templates', 'kp-template.md');

// ---- 解析参数 ----
const argv = process.argv.slice(2);
const opts = {};
for (let i = 0; i < argv.length; i++) {
  const a = argv[i];
  if (!a.startsWith('--')) continue;
  const eq = a.indexOf('=');
  if (eq > 0) { opts[a.slice(2, eq)] = a.slice(eq + 1); }
  else if (argv[i + 1] && !argv[i + 1].startsWith('--')) { opts[a.slice(2)] = argv[++i]; }
  else { opts[a.slice(2)] = true; }
}

const fail = msg => { console.error('❌ ' + msg); process.exit(1); };
const id = opts.id;
const title = opts.title;
const category = opts.category;
const subcategory = opts.subcategory;
if (!id || !title || !category || !subcategory) {
  fail('缺少参数。至少需要 --id --title --category --subcategory（用 --help 看完整用法）');
}
if (!/^[a-z0-9][a-z0-9-]*$/.test(id)) fail(`id "${id}" 不合法：只能用小写字母/数字/连字符，且以字母数字开头`);
if (id !== path.basename(id)) fail('id 里不能有路径分隔符');

const subSub = opts.subSubcategory || '';
const level = (opts.level || 'A').toUpperCase();
const tags = (opts.tags ? String(opts.tags).split(/[,，]/).map(s => s.trim()).filter(Boolean) : []);
const timeC = opts.time || 'O(?)';
const spaceC = opts.space || 'O(?)';

// 路径里不能出现的字符
const safe = s => String(s).replace(/[\\/:*?"<>|]/g, '').trim();
const codePath = opts.code
  ? String(opts.code)
  : [category, subcategory, safe(title), 'Untitled1.cpp'].join('\\');

const mdPath = path.join(CONTENT, id + '.md');
const cppPath = path.join(ROOT, codePath.replace(/[\\/]/g, path.sep));

// ---- 前置检查 ----
if (fs.existsSync(mdPath) && !opts.force) fail(`${path.relative(ROOT, mdPath)} 已存在（要覆盖请加 --force）`);
if (fs.existsSync(cppPath) && !opts.force) fail(`${codePath} 已存在（要覆盖请加 --force）`);
if (!fs.existsSync(TPL)) fail('找不到 templates/kp-template.md');

let graph = { categories: {}, subcategories: {}, subSubcategories: {}, knowledgePoints: {} };
if (fs.existsSync(GRAPH)) {
  try { graph = Object.assign(graph, JSON.parse(fs.readFileSync(GRAPH, 'utf8'))); }
  catch (e) { fail('knowledge_graph.json 解析失败：' + e.message); }
}
if (graph.categories && Object.keys(graph.categories).length && !graph.categories[category]) {
  console.warn(`⚠️  大类「${category}」不在 knowledge_graph.json 的 categories 中，稍后请补 description`);
}
const subKey = `${category}|${subcategory}`;
if (graph.subcategories && Object.keys(graph.subcategories).length && !graph.subcategories[subKey]) {
  console.warn(`⚠️  子分类「${subKey}」未登记，稍后请补 description`);
}

// ---- 生成 md ----
const tpl = fs.readFileSync(TPL, 'utf8');
const md = tpl
  .replace(/\{\{id\}\}/g, id)
  .replace(/\{\{title\}\}/g, title)
  .replace(/\{\{category\}\}/g, category)
  .replace(/\{\{subcategory\}\}/g, subcategory)
  .replace(/\{\{subSubcategory\}\}/g, subSub)
  .replace(/\{\{tags\}\}/g, tags.map(t => `"${t}"`).join(', '))
  .replace(/\{\{timeComplexity\}\}/g, timeC)
  .replace(/\{\{spaceComplexity\}\}/g, spaceC)
  .replace(/\{\{codePath\}\}/g, codePath)
  .replace(/\{\{level\}\}/g, level);

// ---- 代码文件：只写任务卡，不写实现 ----
const cppCard = `/*
 * ${title}
 * 分类：${category} / ${subcategory}${subSub ? ' / ' + subSub : ''}
 *
 * ⏳ 本文件由【你本人】实现，AI 不代写。
 *
 * 实现前请先写清下面四项，再动手：
 *   输入格式：
 *   输出格式：
 *   数据范围：
 *   复杂度目标：时间 ${timeC}，空间 ${spaceC}
 *
 * 写完后：
 *   1) node tools/lint.js content/${id}.md      # 检查元数据与章节
 *   2) 把完整代码同步到 content/${id}.md 的「逐行代码解析」
 *   3) node build.js                            # 重新生成 data.js
 */
`;

if (opts['dry-run']) {
  console.log('🔎 dry-run，不写文件：');
  console.log('   将创建 ' + path.relative(ROOT, mdPath));
  console.log('   将创建 ' + codePath);
  console.log('   将注册 knowledge_graph.json 的 ' + id);
  process.exit(0);
}

fs.mkdirSync(CONTENT, { recursive: true });
fs.writeFileSync(mdPath, md, 'utf8');
fs.mkdirSync(path.dirname(cppPath), { recursive: true });
fs.writeFileSync(cppPath, cppCard, 'utf8');

// ---- 注册知识图谱 ----
if (!opts['no-graph']) {
  graph.knowledgePoints = graph.knowledgePoints || {};
  if (!graph.knowledgePoints[id]) {
    graph.knowledgePoints[id] = { prerequisites: [], related: [] };
    fs.writeFileSync(GRAPH, JSON.stringify(graph, null, 4) + '\n', 'utf8');
    console.log('📖 已在 knowledge_graph.json 注册（prerequisites/related 为空，需你确认后补）');
  } else {
    console.log('📖 knowledge_graph.json 中已存在该 id，未改动');
  }
}

console.log('\n✅ 已创建：');
console.log('   ' + path.relative(ROOT, mdPath));
console.log('   ' + codePath + '   ← 只含任务卡注释，请你自己实现');
console.log('\n下一步：');
console.log(`   1. 填写 content/${id}.md 的讲解（章节已按模板排好，占位处标了「待补充」）`);
console.log(`   2. 实现 ${codePath}`);
console.log(`   3. 把代码同步进「逐行代码解析」，然后：node tools/lint.js content/${id}.md && node build.js`);
console.log('   4. 如需前置/相关知识点，编辑 knowledge_graph.json');
