# AGENTS.md · 算法知识点知识库

> **本文件是跨工具的项目规则**（Trae / Claude Code / Codex / Cursor 都能读）。
> 在 Trae 中让它生效：**设置 → 规则 → 打开「将 AGENTS.md 包含在上下文中」**。
> 完整结构与操作手册见 [`PROJECT_FRAMEWORK.md`](PROJECT_FRAMEWORK.md)；项目说明见 [`README.md`](README.md)。

## 项目是什么

个人维护的**算法/知识点查询网站**（当前主体是 OI 算法），纯静态、GitHub Pages 托管。
内容源是 Markdown，`build.js` 生成前端要用的数据文件。

- 项目根：`C:\Users\penti\Desktop\算法 - 副本`
- 知识点：`content/*.md`（109 篇）｜题目：`problems/*.md`（20 道）
- 代码：各分类目录下的 `.cpp`（由 `codePath` 指向，**用户本人编写**）
- 图谱：`knowledge_graph.json`
- 构建：`node build.js` → `data.js` / `code_data.js` / `problems_data.js`
- 门禁：`node tools/lint.js`（error 必须为 0）

## 三条铁律

1. **不代写解题代码。** 所有 `.cpp`（知识点示例、题目正解、暴力程序、造数据程序）由用户本人编写。
   允许：讲思路、指出 bug 与修复方向、列边界清单、写对拍脚本框架、review 代码、删除调试输出。
   禁止：生成或补全这些 `.cpp` 的实现，或在正文里写出完整可提交代码。
   例外：用户明确要求"破例帮我写"时，回复首行标注 `⚠️ 已按你的破例要求代写代码`。
2. **改完必须过门禁。** `node tools/lint.js`（不能有 error）→ `node build.js`。
3. **内容源是唯一真相。** `data.js` / `code_data.js` / `problems_data.js` 是构建产物，**不要手改**。

## 四个能力（按触发语找对应技能）

| 用户说什么 | 做什么 | 技能文件 |
|---|---|---|
| "这是新知识点，帮我入库" | 添加知识点 | `.trae/skills/add-knowledge/SKILL.md` |
| "这题加进题目专辑" | 添加题目 | `.trae/skills/add-problem/SKILL.md` |
| "XX 那条写错了 / 复杂度不对 / 样例应改为…" | 修改知识点或题目 | `.trae/skills/edit-content/SKILL.md` |
| "更新网站 / 上传 / 提交到 GitHub" | 构建 + 发布 | `.trae/skills/release-github/SKILL.md` |

## 用户的输入习惯（技能都按这个约定）

用户会丢**一个文件夹**过来，通常三个文件：

```
任意文件夹/
├── 题目.md / 题目.txt     ← 只有题目才有；知识点没有这个文件
├── 代码.cpp               ← 用户本人写的实现（初赛笔记类可无）
└── 思路.md / 思路.txt     ← 用户的思路总结（网页版 AI 总结出的笔记）
```

- 文件名不固定，**按内容判断**哪个是题面、代码、思路。
- 代码**只搬运不修改实现**；题面**忠实转述，不补数据范围/样例**。
- 修改类需求是"用户指出哪里错了"：最小改动 + 同步正文代码块。

## 常用命令

```bash
node tools/lint.js            # 内容门禁（error 必须 0）
node tools/lint.js --quiet    # 只看汇总
node build.js                 # 生成三个产物

# 脚手架（生成 md 骨架 + 只含任务卡注释的代码文件 + 图谱空条目）
node tools/new-kp.js --id dp-interval-stone --title "区间DP·石子合并" --category 动态规划 --subcategory 区间DP --level A
node tools/new-problem.js --id prob-luogu-p1050 --title "P1050 循环" --oj 洛谷 --problemId P1050 --difficulty 中等 --category 数学

# 对拍（用户自己写好 gen.cpp / brute.cpp 后）
tools\stress.cmd
```

## 汇报格式（固定四段）

```
【改动】文件列表
【验证】node tools/lint.js → error N / warning M；node build.js → X 知识点 / Y 题目
【需要你本人做】例如：实现 xxx.cpp / 确认某处结论 / 提供题号
【待确认】我做了判断的地方（写清依据与回退方式）
```

## 环境注意

- 本机 PATH 里可能没有 `node`；Trae/终端里能跑就用，否则装 Node LTS 或用绝对路径。
- `g++`：`C:\CSP_SIM\tools\mingw64\bin\g++.exe`（对拍、编译验证用）。
- PowerShell 执行策略是 Restricted：跑 `.ps1` 用 `tools\stress.cmd` 包装，或加 `-ExecutionPolicy Bypass`。
- `.trae/skills/*.ps1` 之类脚本保持 **ASCII + CRLF**，否则 Windows PowerShell 5.1 会读成乱码。
- GitHub Pages 有约 10 分钟 CDN 缓存（`max-age=600`），push 后别立刻断言"线上没变"。

## 细节去哪查

| 想知道 | 看 |
|---|---|
| frontmatter 字段表、章节规范、命名约定、KaTeX 陷阱 | `PROJECT_FRAMEWORK.md` §3 |
| lint 规则含义与常见违规 | `PROJECT_FRAMEWORK.md` §6 |
| 内容 A/B/C 级底线 | `docs/内容质量标准.md` |
| 在网页版提问/总结成笔记的提示词 | `docs/网页版总结提示词.md` |
| 让新会话快速读懂本站的入项提示词 + 理解自检 | `docs/让AI了解本站-提示词.md` |
| 四个技能的触发词与调用模板（用户视角） | `docs/四个技能-调用提示词.md` |
| 项目怎么跑、目录都是什么 | `README.md` |
