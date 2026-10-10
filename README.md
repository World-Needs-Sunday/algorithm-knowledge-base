# 算法知识点知识库 · 项目说明

> 一个个人维护的**算法/知识点查询网站**：内容源是 Markdown，用 `build.js` 生成前端数据，纯静态托管在 GitHub Pages。
> **给 AI 的一句话**：先读 [`AGENTS.md`](AGENTS.md)（铁律 + 入口），再读 [`PROJECT_FRAMEWORK.md`](PROJECT_FRAMEWORK.md)（完整框架与操作手册），然后按 `.trae/skills/` 里对应的技能执行。

---

## 1. 这是什么

- **当前规模**：知识点 **109** 篇 · 题目 **20** 道 · **8** 个大类（基础算法 / 动态规划 / 数据结构 / 图论 / 数学 / 字符串 / 输入输出 / 初赛笔记）
- **形态**：单文件前端 `index.html`（自带 CSS + JS）+ 三个构建产物，**没有框架、没有 npm 依赖、没有后端**
- **在线**：GitHub Pages —— 仓库 `https://github.com/World-Needs-Sunday/algorithm-knowledge-base.git`（分支 `master`）
- **本地**：`C:\Users\penti\Desktop\算法 - 副本`

## 2. 怎么跑起来

```bash
# 1) 看网页：直接双击 index.html 即可（数据用 <script> 引入，不受 file:// 限制）
# 2) 改完内容后（每次都要）：
node tools/lint.js      # 内容门禁：error 必须为 0
node build.js           # 生成 data.js / code_data.js / problems_data.js
# 3) 浏览器 Ctrl+F5 强制刷新（data.js 会被缓存）
```

> 本机可能没把 `node` 加进 PATH：Trae/终端里能直接跑就用 `node`；否则用绝对路径或先装 Node LTS。
> `g++`（对拍用）：`C:\CSP_SIM\tools\mingw64\bin\g++.exe`。

## 3. 数据流（一张图记住）

```
content/*.md ─┐
problems/*.md ─┼─► node build.js ─┬─► data.js          KNOWLEDGE_DATA + 分类元数据
knowledge_graph.json ─┘            ├─► problems_data.js PROBLEMS_DATA
各分类/*.cpp ──────────────────────┴─► code_data.js     CODE_DATA（按 codePath 索引）
                                                   │
                                                   ▼
                    index.html（KaTeX 三步法渲染公式 + highlight.js 高亮代码）
```

**产物不可手改**：`data.js` / `code_data.js` / `problems_data.js` 是构建产物，下次构建会被覆盖。**唯一真相是 `content/`、`problems/`、`knowledge_graph.json`、各分类 `.cpp`。**

## 4. 目录速览

| 路径 | 是什么 |
|---|---|
| `index.html` | 前端单文件（页面布局、KaTeX 三步法、标签筛选、搜索、hash 路由） |
| `content/*.md` | **知识点源文件**（frontmatter + 7/8 章正文） |
| `problems/*.md` | **题目源文件**（frontmatter + 题面/思路/代码解析） |
| `knowledge_graph.json` | 分类描述 + 前置/相关关系（`prerequisites` / `related`） |
| `各分类目录/*.cpp` | **你本人写的代码**（知识点示例、题目正解、暴力、造数据） |
| `build.js` | 构建脚本（扫描 → 解析 frontmatter → 读 .cpp → 生成三个产物） |
| `tools/lint.js` | **内容门禁**（E0xx 错误必须清零；W0xx 建议修） |
| `tools/new-kp.js` / `new-problem.js` | 脚手架：生成 md 骨架 + 只含任务卡注释的代码文件 + 注册图谱 |
| `templates/` | 知识点 / 题目模板 |
| `docs/内容质量标准.md` | A/B/C 三级标准与硬性底线 |
| `docs/网页版总结提示词.md` | 在网页版（DeepSeek 等）提问与总结成笔记的提示词 |
| `docs/让AI了解本站-提示词.md` | **给 AI 的入项提示词**（让新会话快速读懂本站并会干活）+ 理解自检 5 问 |
| `docs/四个技能-调用提示词.md` | **四个技能怎么"喊"**：触发词、可照抄的调用模板、兜底句、元数据速答卡 |
| `.trae/skills/` | **四个能力技能**（见下） |
| `.trae/rules/project_rules.md` | Trae 原生项目规则 |
| `AGENTS.md` / `PROJECT_FRAMEWORK.md` | AI 入口 / 框架与操作手册 |

## 5. 四个能力（都能直接让 AI 执行）

| 能力 | 你说什么 | 技能文件 |
|---|---|---|
| **添加知识点** | "这个文件夹是新的知识点，帮我入库" | [`.trae/skills/add-knowledge/SKILL.md`](.trae/skills/add-knowledge/SKILL.md) |
| **添加题目** | "这题帮我加进题目专辑" | [`.trae/skills/add-problem/SKILL.md`](.trae/skills/add-problem/SKILL.md) |
| **修改知识点 / 题目** | "XX 那条写错了，应该是……" | [`.trae/skills/edit-content/SKILL.md`](.trae/skills/edit-content/SKILL.md) |
| **构建并上传 GitHub** | "改完了，更新网站 / 上传" | [`.trae/skills/release-github/SKILL.md`](.trae/skills/release-github/SKILL.md) |

**我的输入习惯（技能都按这个约定）**：我会丢**一个文件夹**过来，里面通常三个文件 ——

```
任意文件夹/
├── 题目.md / 题目.txt     ← 只有题目才有；知识点没有这个文件
├── 代码.cpp               ← 我本人写的实现（初赛笔记类知识点可以没有）
└── 思路.md / 思路.txt     ← 我的思路总结（一般是网页版总结出来的笔记）
```

修改类需求则是：**我发现哪里错了，把错的地方和正确说法发给你**。

## 6. 三条铁律（违反任何一条都是失败）

1. **不代写解题代码**：所有 `.cpp`（知识点示例、题目正解、暴力、造数据）由我本人写。AI 只做讲解、审查、指出 bug 与方向、写对拍脚本框架。
2. **改完必须过门禁**：`node tools/lint.js`（error 必须 0）→ `node build.js`。
3. **内容源是唯一真相**：`data.js` / `code_data.js` / `problems_data.js` 是产物，永不手改。

细则、字段表、章节规范、常见陷阱全部在 [`PROJECT_FRAMEWORK.md`](PROJECT_FRAMEWORK.md)。

## 7. 想在新环境继续维护

需要的最小集合：本仓库 + Node（跑 `build.js`/`tools/*.js`）。
如果 `.trae/skills/` 不被你的工具识别（比如换到 Claude Code / Codex），把 `AGENTS.md` 作为规则文件喂给 AI 即可——它会把 AI 引到 `.trae/skills/` 与 `PROJECT_FRAMEWORK.md`。
