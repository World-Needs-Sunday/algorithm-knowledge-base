#!/usr/bin/env node
/**
 * tools/new-problem.js — 题目脚手架
 *
 * 生成：problems/<id>.md（按 templates/problem-template.md）
 *      题目代码文件（**只放任务卡注释，不含任何实现**）
 *
 * 用法：
 *   node tools/new-problem.js --id prob-p1234-xxx --title "P1234 xxx — 差分贪心" \
 *        --oj 洛谷 --problemId P1234 --difficulty 中等 --category 数学 \
 *        [--tags "差分,贪心"] [--time "O(n)"] [--space "O(n)"] [--level B] \
 *        [--code "题目专辑\数学\xxx\代码.cpp"] [--force] [--dry-run]
 */

'use strict';
const fs = require('fs');
const path = require('path');

const ROOT = path.resolve(__dirname, '..');
const PROBLEMS = path.join(ROOT, 'problems');
const TPL = path.join(ROOT, 'templates', 'problem-template.md');

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
const id = opts.id, title = opts.title, category = opts.category;
if (!id || !title || !category) fail('缺少参数。至少需要 --id --title --category（见 --help）');
if (!/^[a-z0-9][a-z0-9-]*$/.test(id)) fail(`id "${id}" 不合法：小写字母/数字/连字符`);
const difficulty = opts.difficulty || '中等';
const DIFF = ['入门', '简单', '中等', '困难', '提高'];
if (!DIFF.includes(difficulty)) fail(`难度「${difficulty}」不在约定枚举：${DIFF.join(' / ')}`);

const safe = s => String(s).replace(/[\\/:*?"<>|]/g, '').trim();
// 代码目录短名：去掉题号前缀与副标题
const shortName = opts.codeName
  ? safe(opts.codeName)
  : safe(String(title).replace(/^[A-Za-z]*\d+\s*/, '').split(/[—\-–]/)[0]).slice(0, 24) || id;
const codePath = opts.code ? String(opts.code) : ['题目专辑', category, shortName, '代码.cpp'].join('\\');

const mdPath = path.join(PROBLEMS, id + '.md');
const cppPath = path.join(ROOT, codePath.replace(/[\\/]/g, path.sep));

if (fs.existsSync(mdPath) && !opts.force) fail(`${path.relative(ROOT, mdPath)} 已存在（要覆盖请加 --force）`);
if (fs.existsSync(cppPath) && !opts.force) fail(`${codePath} 已存在（要覆盖请加 --force）`);
if (!fs.existsSync(TPL)) fail('找不到 templates/problem-template.md');

const tags = opts.tags ? String(opts.tags).split(/[,，]/).map(s => s.trim()).filter(Boolean) : [];
const timeC = opts.time || 'O(?)', spaceC = opts.space || 'O(?)', level = (opts.level || 'A').toUpperCase();

const md = fs.readFileSync(TPL, 'utf8')
  .replace(/\{\{id\}\}/g, id)
  .replace(/\{\{title\}\}/g, title)
  .replace(/\{\{oj\}\}/g, opts.oj || '')
  .replace(/\{\{problemId\}\}/g, opts.problemId || '')
  .replace(/\{\{difficulty\}\}/g, difficulty)
  .replace(/\{\{category\}\}/g, category)
  .replace(/\{\{tags\}\}/g, tags.map(t => `"${t}"`).join(', '))
  .replace(/\{\{timeComplexity\}\}/g, timeC)
  .replace(/\{\{spaceComplexity\}\}/g, spaceC)
  .replace(/\{\{codePath\}\}/g, codePath)
  .replace(/\{\{level\}\}/g, level);

const cppCard = `/*
 * ${title}
 * 来源：${opts.oj || '(待确认)'} ${opts.problemId || ''}   难度：${difficulty}
 *
 * ⏳ 本文件由【你本人】实现，AI 不代写。
 *
 * 实现前请先写清：输入格式 / 输出格式 / 数据范围 / 复杂度目标（时间 ${timeC}，空间 ${spaceC}）
 *
 * 写完后：
 *   1) 把样例真跑一遍，确认输出与题面一致
 *   2) 把完整代码同步到 problems/${id}.md 的「逐行代码解析」
 *   3) node tools/lint.js problems/${id}.md && node build.js
 *   4) 需要验正确性时：把 gen.cpp / brute.cpp 放到同目录，跑 tools\\stress.cmd -Dir "<本目录>"
 */
`;

if (opts['dry-run']) {
  console.log('🔎 dry-run，不写文件：');
  console.log('   将创建 ' + path.relative(ROOT, mdPath));
  console.log('   将创建 ' + codePath);
  process.exit(0);
}

fs.mkdirSync(PROBLEMS, { recursive: true });
fs.writeFileSync(mdPath, md, 'utf8');
fs.mkdirSync(path.dirname(cppPath), { recursive: true });
fs.writeFileSync(cppPath, cppCard, 'utf8');

console.log('\n✅ 已创建：');
console.log('   ' + path.relative(ROOT, mdPath));
console.log('   ' + codePath + '   ← 只含任务卡注释，请你自己实现');
console.log('\n下一步：');
console.log('   1. 填题面（描述/输入/输出/数据范围）与样例——样例必须是真实的，别凭记忆写');
console.log(`   2. 实现 ${codePath}，再把代码同步进「逐行代码解析」`);
console.log(`   3. node tools/lint.js problems/${id}.md && node build.js`);
