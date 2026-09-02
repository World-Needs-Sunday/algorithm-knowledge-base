/**
 * ====================================================================
 *   算法知识点查询系统 - 构建脚本 (build.js)
 * ====================================================================
 *
 *   用法：node build.js
 *
 *   功能：
 *     1. 扫描 content/ 目录下所有 .md 文件（知识点元数据 + 完整讲解）
 *     2. 读取对应的 .cpp 代码文件（自动检测 UTF-8/GBK 编码）
 *     3. 生成 data.js 和 code_data.js 供 index.html 使用
 *
 *   添加新知识点：
 *     1. 在 content/ 目录新建一个 .md 文件（格式见下方说明）
 *     2. 将对应的 .cpp 代码文件放到算法目录结构中
 *     3. 运行 node build.js
 *
 *   Markdown 文件格式：
 *     ---
 *     id: dp-01bag-standard
 *     title: '01背包（标准）'
 *     category: 动态规划
 *     subcategory: 背包问题
 *     tags: ["动态规划", "背包问题", "01背包", "DP"]
 *     timeComplexity: 'O(n×m)'
 *     spaceComplexity: 'O(n×m)'
 *     codePath: '动态规划\背包问题模板\01背包\标准\Untitled1.cpp'
 *     ---
 *
 *     ## 算法原理
 *     （正文内容，支持完整 Markdown 语法）
 *
 *     ## 核心公式
 *     （使用 $...$ 行内公式，$$...$$ 块级公式）
 *
 *     ## 适用场景
 *     （场景描述...）
 *
 *     ## 常见陷阱与注意事项
 *     - 要点1
 *     - 要点2
 *
 *   注意：body 字段存储完整的 Markdown 正文，由前端 marked.js 渲染。
 *         公式使用 LaTeX 语法（KaTeX 渲染）。
 * ====================================================================
 */

const fs = require('fs');
const path = require('path');
const { TextDecoder } = require('util');

const ROOT = __dirname;
const CONTENT_DIR = path.join(ROOT, 'content');
const PROBLEMS_DIR = path.join(ROOT, 'problems');
const OUTPUT_DATA = path.join(ROOT, 'data.js');
const OUTPUT_CODE = path.join(ROOT, 'code_data.js');
const OUTPUT_PROBLEMS = path.join(ROOT, 'problems_data.js');
const GRAPH_FILE = path.join(ROOT, 'knowledge_graph.json');

// ===== 解析 YAML Frontmatter =====
function parseFrontmatter(text) {
    const fmMatch = text.match(/^---\r?\n([\s\S]*?)\r?\n---\r?\n([\s\S]*)$/);
    if (!fmMatch) return { meta: {}, body: text };

    const yamlText = fmMatch[1];
    const body = fmMatch[2];
    const meta = {};

    yamlText.split('\n').forEach(line => {
        const m = line.match(/^(\w+):\s*(.*)$/);
        if (!m) return;
        const key = m[1];
        let val = m[2].trim();

        // 数组格式: [a, b, c]
        if (val.startsWith('[') && val.endsWith(']')) {
            val = val.slice(1, -1).split(',').map(s => s.trim().replace(/^["']|["']$/g, ''));
        } else {
            // 去除外层引号（单引号或双引号）
            val = val.replace(/^["']|["']$/g, '');
        }
        meta[key] = val;
    });

    return { meta, body };
}

// ===== 检测字节数组是否为合法 UTF-8 =====
function isUtf8(bytes) {
    for (let i = 0; i < bytes.length; i++) {
        const b = bytes[i];
        if (b < 0x80) continue;
        if (b >= 0xC2 && b <= 0xDF) {
            if (++i >= bytes.length || (bytes[i] & 0xC0) !== 0x80) return false;
        } else if (b >= 0xE0 && b <= 0xEF) {
            if (i + 2 >= bytes.length || (bytes[i + 1] & 0xC0) !== 0x80 || (bytes[i + 2] & 0xC0) !== 0x80) return false;
            i += 2;
        } else if (b >= 0xF0 && b <= 0xF4) {
            if (i + 3 >= bytes.length || (bytes[i + 1] & 0xC0) !== 0x80 || (bytes[i + 2] & 0xC0) !== 0x80 || (bytes[i + 3] & 0xC0) !== 0x80) return false;
            i += 3;
        } else {
            return false;
        }
    }
    return true;
}

// ===== 批量读取代码文件（自动检测 UTF-8/GBK 编码）=====
const gbkDecoder = new TextDecoder('gbk');
function readCodeFiles(codePaths) {
    const codeData = {};

    codePaths.forEach(p => {
        const fullPath = path.join(ROOT, p);
        try {
            const bytes = fs.readFileSync(fullPath);
            if (isUtf8(bytes)) {
                codeData[p] = bytes.toString('utf8');
            } else {
                codeData[p] = gbkDecoder.decode(bytes);
            }
        } catch (e) {
            codeData[p] = '// 代码文件未找到: ' + p;
        }
    });

    return codeData;
}

// ===== 主构建流程 =====
function build() {
    console.log('🔍 扫描 content/ 目录...');

    if (!fs.existsSync(CONTENT_DIR)) {
        console.error('❌ content/ 目录不存在！请先创建该目录并放入 .md 文件。');
        process.exit(1);
    }

    // 读取知识图谱（前置知识点 + 相关知识点 + 分类描述）
    let graph = { categories: {}, subcategories: {}, knowledgePoints: {} };
    if (fs.existsSync(GRAPH_FILE)) {
        graph = JSON.parse(fs.readFileSync(GRAPH_FILE, 'utf8'));
        console.log(`📖 加载知识图谱: ${Object.keys(graph.knowledgePoints).length} 个知识点关系`);
    } else {
        console.warn('⚠️  knowledge_graph.json 不存在，跳过知识图谱');
    }

    const mdFiles = fs.readdirSync(CONTENT_DIR).filter(f => f.endsWith('.md')).sort();
    console.log(`📄 找到 ${mdFiles.length} 个 Markdown 文件`);

    const knowledgeData = [];
    const allCodePaths = [];
    const codePathToKpIndex = {};
    let warnings = [];

    // 解析所有 .md 文件
    mdFiles.forEach(filename => {
        const filepath = path.join(CONTENT_DIR, filename);
        const raw = fs.readFileSync(filepath, 'utf8');
        const { meta, body } = parseFrontmatter(raw);

        // 校验必填字段
        const required = ['id', 'title', 'category', 'subcategory', 'codePath'];
        for (const field of required) {
            if (!meta[field]) {
                warnings.push(`⚠️  ${filename}: 缺少字段 "${field}"`);
            }
        }

        const codePath = meta.codePath || '';
        if (codePath) {
            allCodePaths.push(codePath);
            codePathToKpIndex[codePath] = knowledgeData.length;
        }

        // 存储完整 Markdown 正文（由前端 marked.js 渲染）
        const kpId = meta.id || path.basename(filename, '.md');
        const graphEntry = graph.knowledgePoints[kpId] || {};
        knowledgeData.push({
            id: kpId,
            category: meta.category || '未分类',
            subcategory: meta.subcategory || '未分类',
            title: meta.title || meta.id || filename,
            content: body.trim(),
            timeComplexity: meta.timeComplexity || '未知',
            spaceComplexity: meta.spaceComplexity || '未知',
            tags: Array.isArray(meta.tags) ? meta.tags : (meta.tags ? [meta.tags] : []),
            codePath: codePath,
            prerequisites: graphEntry.prerequisites || [],
            related: graphEntry.related || []
        });

        console.log(`  ✓ ${meta.id || filename} — ${meta.title || ''}`);
    });

    // 读取所有代码文件（知识点 + 题目）
    console.log(`\n📖 读取 ${allCodePaths.length} 个代码文件...`);
    const codeData = readCodeFiles(allCodePaths);

    // 验证代码文件是否存在
    let codeFound = 0;
    allCodePaths.forEach(p => {
        if (codeData[p] && !codeData[p].startsWith('// 代码文件未找到')) {
            codeFound++;
        } else {
            warnings.push(`⚠️  代码文件不存在: ${p}`);
        }
    });
    console.log(`   成功读取 ${codeFound}/${allCodePaths.length} 个代码文件`);

    // ===== 扫描题目专辑 =====
    console.log('\n📝 扫描 problems/ 目录...');

    let problemsData = [];
    if (fs.existsSync(PROBLEMS_DIR)) {
        const probFiles = fs.readdirSync(PROBLEMS_DIR).filter(f => f.endsWith('.md')).sort();
        console.log(`📄 找到 ${probFiles.length} 个题目 Markdown 文件`);

        probFiles.forEach(filename => {
            const filepath = path.join(PROBLEMS_DIR, filename);
            const raw = fs.readFileSync(filepath, 'utf8');
            const { meta, body } = parseFrontmatter(raw);

            const probId = meta.id || path.basename(filename, '.md');
            const codePath = meta.codePath || '';
            if (codePath) {
                allCodePaths.push(codePath);
            }

            problemsData.push({
                id: probId,
                title: meta.title || probId,
                oj: meta.oj || '',
                problemId: meta.problemId || '',
                difficulty: meta.difficulty || '未标注',
                category: meta.category || '未分类',
                tags: Array.isArray(meta.tags) ? meta.tags : (meta.tags ? [meta.tags] : []),
                timeComplexity: meta.timeComplexity || '未知',
                spaceComplexity: meta.spaceComplexity || '未知',
                codePath: codePath,
                content: body.trim()
            });

            console.log(`  ✓ ${probId} — ${meta.title || ''}`);
        });

        // 读取题目代码文件（追加到 codeData）
        if (allCodePaths.length > codeFound) {
            const newPaths = allCodePaths.slice(codeFound);
            const newCodeData = readCodeFiles(newPaths);
            Object.assign(codeData, newCodeData);
            let probCodeFound = 0;
            newPaths.forEach(p => {
                if (codeData[p] && !codeData[p].startsWith('// 代码文件未找到')) {
                    probCodeFound++;
                } else {
                    warnings.push(`⚠️  题目代码文件不存在: ${p}`);
                }
            });
            console.log(`   成功读取 ${probCodeFound}/${newPaths.length} 个题目代码文件`);
        }
    } else {
        console.log('   problems/ 目录不存在，跳过题目专辑');
    }

    // 输出警告
    if (warnings.length > 0) {
        console.log('\n⚠️  警告:');
        warnings.forEach(w => console.log('  ' + w));
    }

    // 生成 data.js
    const dataJs = `// 此文件由 build.js 自动生成，请勿手动编辑
// 修改 content/ 下的 .md 文件后运行 node build.js 重新生成
const KNOWLEDGE_DATA = ${JSON.stringify(knowledgeData, null, 4)};

const CATEGORY_META = ${JSON.stringify(graph.categories || {}, null, 4)};

const SUBCATEGORY_META = ${JSON.stringify(graph.subcategories || {}, null, 4)};
`;
    fs.writeFileSync(OUTPUT_DATA, dataJs, 'utf8');
    console.log(`\n✅ 生成 data.js (${knowledgeData.length} 个知识点, ${(dataJs.length / 1024).toFixed(1)} KB)`);

    // 生成 code_data.js
    const codeJs = `// 此文件由 build.js 自动生成，请勿手动编辑
const CODE_DATA = ${JSON.stringify(codeData, null, 4)};
`;
    fs.writeFileSync(OUTPUT_CODE, codeJs, 'utf8');
    console.log(`✅ 生成 code_data.js (${Object.keys(codeData).length} 个代码文件, ${(codeJs.length / 1024).toFixed(1)} KB)`);

    // 生成 problems_data.js
    const problemsJs = `// 此文件由 build.js 自动生成，请勿手动编辑
const PROBLEMS_DATA = ${JSON.stringify(problemsData, null, 4)};
`;
    fs.writeFileSync(OUTPUT_PROBLEMS, problemsJs, 'utf8');
    console.log(`✅ 生成 problems_data.js (${problemsData.length} 个题目, ${(problemsJs.length / 1024).toFixed(1)} KB)`);

    console.log('\n🎉 构建完成！');
}

build();
