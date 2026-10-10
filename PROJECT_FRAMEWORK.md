# 框架文件 · 算法知识点知识库

> **这份文件给谁看**：任何要维护本站的 AI（Trae / Claude Code / Codex / Cursor / 对话式 AI）。
> **读完它能干什么**：快速理解整个网页的结构与数据流，并正确执行四个操作——**添加知识点 / 添加题目 / 修改内容 / 构建上传 GitHub**。
> **配套**：[`README.md`](README.md)（项目说明）、[`AGENTS.md`](AGENTS.md)（铁律入口）、[`docs/内容质量标准.md`](docs/内容质量标准.md)（A/B/C 级底线）、[`docs/让AI了解本站-提示词.md`](docs/让AI了解本站-提示词.md)（新会话入项提示词）、`.trae/skills/`（四个能力的逐步流程）。

---

## 0. 30 秒速览

| 问题 | 答案 |
|---|---|
| 网站是什么 | 纯静态算法/知识点查询站（单文件 `index.html` + 三个构建产物），GitHub Pages 托管 |
| 内容从哪来 | `content/*.md`（知识点 109 篇）、`problems/*.md`（题目 20 道）、各分类目录下的 `.cpp`（你本人写的代码） |
| 怎么生成网页数据 | `node build.js` → `data.js` + `problems_data.js` + `code_data.js` |
| 改了内容怎么验证 | `node tools/lint.js`（**error 必须为 0**）→ `node build.js` → 浏览器 Ctrl+F5 |
| 什么绝对不能做 | ① 代写 `.cpp` 实现 ② 手改三个产物 ③ 不过门禁就交付 |
| 四个能力在哪 | `.trae/skills/add-knowledge`、`add-problem`、`edit-content`、`release-github` |

---

## 1. 三条铁律（最高优先级）

1. **不代写解题代码。**
   所有 `.cpp`（知识点示例、题目正解、暴力程序、造数据程序）由用户本人编写。
   **允许**：讲思路、指出 bug 与修复方向、列边界清单、写对拍脚本框架、review 代码、删除调试输出。
   **禁止**：生成或补全这些 `.cpp` 的实现，或在正文里写出完整可提交代码。
   例外：用户明确说"破例帮我写"时，回复首行标注 `⚠️ 已按你的破例要求代写代码`。
2. **改完必须过门禁。** `node tools/lint.js`（error 必须 0）→ `node build.js`。
3. **内容源是唯一真相。** `data.js` / `code_data.js` / `problems_data.js` 是构建产物，永不手改。

---

## 2. 目录结构与数据流

```
算法 - 副本/
├── index.html              # 前端单文件（布局 + KaTeX 三步法 + 搜索 + 标签筛选 + hash 路由）
├── build.js                # 构建脚本（唯一需要的"构建系统"）
├── knowledge_graph.json    # 分类描述 + prerequisites/related
├── README.md               # 项目说明（人/AI 入口）
├── AGENTS.md               # 三条铁律 + 入口指引
├── PROJECT_FRAMEWORK.md    # 本文件
├── content/                # 知识点源文件（109 篇，平铺）
├── problems/               # 题目源文件（20 道，平铺）
├── 动态规划/ 数据结构/ 图论/ 数学/ 字符串/ 基础算法/ 输入,输出/   # 知识点 .cpp
├── 题目专辑/                # 题目 .cpp
├── templates/              # kp-template.md / problem-template.md
├── tools/                  # lint.js（门禁）+ new-kp.js + new-problem.js + stress.*（对拍）
├── docs/                   # 内容质量标准.md / 网页版总结提示词.md
├── .trae/skills/           # 四个能力技能
├── vendor/                 # 本地前端依赖（KaTeX / marked / highlight.js + 字体）
├── data.js problems_data.js code_data.js   # 【产物】不要手改
└── .github/workflows/      # 内容质检 CI（可选）
```

**数据流**：

```
content/*.md ─┐
problems/*.md ─┼─► build.js ─┬─► data.js          KNOWLEDGE_DATA + CATEGORY_META + SUBCATEGORY_META
knowledge_graph.json ─┘       ├─► problems_data.js PROBLEMS_DATA
各分类/*.cpp ─────────────────┴─► code_data.js     CODE_DATA（key = codePath）
                                     ↓
                          index.html 渲染（公式走 KaTeX 三步法，代码走 highlight.js）
```

**build.js 的关键行为**（写内容时要顺着它）：

| 行为 | 说明 |
|---|---|
| 扫描范围 | `content/*.md`、`problems/*.md`（平铺，不递归子目录） |
| frontmatter 解析 | 正则匹配 `^---\r?\n([\s\S]*?)\r?\n---\r?\n`；逐行 `^(\w+):\s*(.*)$`；数组只支持单行 `[a, b]` |
| 必填字段 | 知识点 `id/title/category/subcategory`；题目 `id/title/difficulty/category`（缺了只警告不中断） |
| 代码编码 | 先检测是否合法 UTF-8，否则按 GBK 解码（所以 UTF-8 / GBK 都能读） |
| 代码缺失 | `codePath` 指向的文件不存在 → 产物里写入 `// 代码文件未找到: xxx` 并告警 |
| 图谱合并 | `prerequisites` / `related` 只从 `knowledge_graph.json` 读，**不读 .md frontmatter** |

---

## 3. 内容规范

### 3.1 知识点 frontmatter（`content/<id>.md`）

| 字段 | 必填 | 说明 |
|---|---|---|
| `id` | ✅ | **必须等于文件名（去 .md）**，小写字母/数字/连字符 |
| `title` | ✅ | 正式名称，单引号包裹；**禁止**"自己写的""以前没学过""D. xxx"这类字样 |
| `category` | ✅ | 8 个之一：基础算法 / 动态规划 / 数据结构 / 图论 / 数学 / 字符串 / 输入输出 / 初赛笔记 |
| `subcategory` | ✅ | 子分类，需在 `knowledge_graph.json` 的 `subcategories` 里登记 |
| `subSubcategory` | ❌ | 小分类（需在 `subSubcategories` 登记） |
| `tags` | ❌ | 3–6 个最佳，**最多 8 个** |
| `timeComplexity` / `spaceComplexity` | ❌ | **不要写"未知"**；数值要与正文、与实现三者一致 |
| `codePath` | ❌ | 反斜杠相对路径，指向真实存在的 `.cpp`；初赛笔记类可省略 |
| `level` | ❌ | `A` / `B` / `C`（见 `docs/内容质量标准.md`；缺省按 A 处理） |
| `verified` | ❌ | 最近人工复核日期 `YYYY-MM-DD` |

### 3.2 题目 frontmatter（`problems/<id>.md`）

| 字段 | 必填 | 说明 |
|---|---|---|
| `id` | ✅ | 等于文件名；建议 `prob-<oj缩写>-<题号>`，如 `prob-luogu-p1050`、`prob-cf-2252d` |
| `title` | ✅ | 题名；**去掉 `D.`/`C1.` 这类场次编号前缀** |
| `oj` | ✅ | 洛谷 / Codeforces / AtCoder / ICPC / CSP-S …；查不到就写 `来源未考证`，**不许编** |
| `problemId` | ✅ | `P1050` / `CF2252D` …；查不到写 `-` |
| `difficulty` | ✅ | 入门 / 简单 / 中等 / 困难 / 提高 |
| `category` | ✅ | 侧边栏分组用（按考点大类填） |
| `tags` / 复杂度 / `codePath` / `level` | ❌ | 同知识点 |

### 3.3 正文章节

**算法类知识点（7 章，标题逐字用）**

```
## 算法原理
## 核心公式 / 状态定义与转移方程
## 逐行代码解析
## 复杂度分析
## 适用场景
## 常见陷阱与注意事项
## 对比与扩展
```

**数据结构类知识点（在「核心公式」后插入两章，共 8–9 章）**

```
## 数据结构图示        ← ASCII 图，展示结构布局与操作过程（数据结构类必须）
## 核心操作详解        ← 逐个操作分小节（偏结构型；偏数学型可并入公式章）
```

**题目（章节名同样逐字用）**

```
## 题目描述 → ### 输入格式 / ### 输出格式 / ### 数据范围   ← 数据范围必须写
## 样例           ← 必须有（`### 样例 1` + 输入/输出/解释）
## 解题思路       ← 核心章节：从"看到题想到什么"讲到"为什么这个做法对"
## 逐行代码解析
## 复杂度分析
## 常见陷阱与注意事项
## 对比与扩展
```

**初赛笔记类**：章节自由（按考点组织），无 `codePath`、可无复杂度。

### 3.4 代码文件约定

| 项 | 约定 |
|---|---|
| 知识点代码路径 | `{大类}\{子分类}\{算法名}\Untitled1.cpp` |
| 题目代码路径 | `题目专辑\{分类}\{题名}\代码.cpp` |
| `codePath` 写法 | **反斜杠**、相对项目根、不加前导 `\`，例：`'图论\最短路\Johnson\Untitled1.cpp'` |
| 编码 | UTF-8 推荐（GBK 也能读） |
| 正文与代码 | 「逐行代码解析」里的**整段代码必须与 `.cpp` 逐字一致**（lint W028）；只放关键片段 + 逐行说明同样合格 |
| 提交前 | 搜一遍 `printf` / `cout`，确认没有调试/计时输出（lint W030；残留会 WA） |

### 3.5 知识图谱（`knowledge_graph.json`）

```jsonc
{
  "categories":      { "图论": { "nature": "…", "description": "…", "problemDomain": "…" } },
  "subcategories":   { "图论|最短路": { "description": "…" } },
  "subSubcategories":{ "图论|最短路|单源最短路": { "description": "…" } },
  "knowledgePoints": {
    "graph-dijkstra-heap": { "prerequisites": ["ds-uf-basic"], "related": ["graph-floyd", "graph-spfa"] }
  }
}
```

- 新增知识点：必须在 `knowledgePoints` 加条目（否则 lint W013）；新增子分类/小分类要同时补 `subcategories` / `subSubcategories` 描述。
- `related` **保持双向**（A 指向 B，B 也指向 A），否则 lint W033 会提示不对称。
- `prerequisites` / `related` 只能用**已存在的知识点 id**（否则 E011）。

### 3.6 命名约定

| 前缀 | 大类 | 例 |
|---|---|---|
| `basic-` | 基础算法 | `basic-quickselect-kth` |
| `dp-` | 动态规划 | `dp-01bag-standard` |
| `ds-` | 数据结构 | `ds-segtree-basic` |
| `graph-` | 图论 | `graph-johnson` |
| `math-` | 数学 | `math-quick-pow-basic` |
| `str-` | 字符串 | `str-kmp` |
| `io-` | 输入输出 | `io-fast-io` |
| `exam-` | 初赛笔记 | `exam-bit-operation` |
| `prob-` | 题目 | `prob-luogu-p1050` |

同算法多版本用后缀区分：`-standard` / `-2d` / `-binary` / `-monotone` / `-template` / `-self`（自己写的）/ `-old`（旧版）。

### 3.7 写作禁忌与渲染陷阱

**禁忌**（lint 会报）：

- 禁止草稿式自我修正：「等等，」「不对，」「纠正一下」「？不，等一下」「我再想想」。
- 禁止标题自述：「自己写的」「以前没有学的时候写的」「未完成」。
- 禁止写"我们重新算一下"式推导过程——思路要用**复盘口吻**（"容易先想到 X，但 X 不行，因为…"）。
- 复杂度不许写"未知"/"待定"。
- 表格里的 `|`：数学公式内必须转义或改写（见下）。
- 不出现 `代紅`（"洛谷"的错误编码）、`锟斤拷`、U+FFFD、`\r\r\n`。

**KaTeX 陷阱**（写公式前必看）：

| 陷阱 | 正确写法 |
|---|---|
| `\text{}` 里的下划线被当成下标 | `\text{\_\_builtin\_clz}`，或干脆避免在 `\text{}` 内用下划线 |
| 表格里公式含 `|` 会把表格拆成多余单元格 | 写成 `\lvert x \rvert`（不要写 `\|`，那是双竖线范数符号） |
| `$` 不成对（如正文里的货币符号） | 成对使用；行内公式 `$` 两侧不要留空格 |
| 反引号代码段里混进 LaTeX | 代码引用与公式分开：`` `fac[i]` ``（即 $2^i \bmod \text{mod}$） |

---

## 4. 四个能力（总览；逐步流程在 `.trae/skills/`）

### 4.1 添加知识点 → `.trae/skills/add-knowledge/SKILL.md`

- **触发**："这是新知识点，帮我入库""把这份总结整理成知识点""文件夹里是代码和思路"
- **输入**：一个文件夹，含 `代码.cpp` + `思路.md`（知识点**没有**题目文件）
- **产出**：`content/<id>.md` + `{大类}\{子分类}\{算法名}\Untitled1.cpp` + 图谱条目
- **验收**：`lint` error 0、`build` 正常、浏览器能看到且公式/代码正常渲染

### 4.2 添加题目 → `.trae/skills/add-problem/SKILL.md`

- **触发**："这题加进题目专辑""文件夹里是题目+代码+思路"
- **输入**：一个文件夹，含 `题目.md` + `代码.cpp` + `思路.md`
- **产出**：`problems/<id>.md` + `题目专辑\{分类}\{题名}\代码.cpp`
- **验收**：同 4.1，另外校验 `oj`/`problemId`/`difficulty`/`数据范围`/`样例` 齐全

### 4.3 修改知识点 / 题目 → `.trae/skills/edit-content/SKILL.md`

- **触发**："XX 那条写错了""这里的复杂度不对""样例输出应该是 -1""代码改了，正文也改一下"
- **输入**：用户的错误描述（可能带截图/原话）
- **产出**：最小改动的 `.md` 和/或 `.cpp` + 同步的正文代码块
- **验收**：lint error 0、build 正常、改动前后 diff 清楚、**没有顺手改别的东西**

### 4.4 构建并上传 GitHub → `.trae/skills/release-github/SKILL.md`

- **触发**："更新网站""上传""提交到 GitHub""发布"
- **步骤**：`lint` → `build` → 前端一致性自检 → `git add/commit/push` → 线上验证（Pages 缓存约 10 分钟）
- **验收**：推送成功、线上四个产物与本地一致、回滚方法已知

---

## 5. 输入约定：用户上传的文件夹

```
任意文件夹名/
├── 题目.md 或 题目.txt      ← 只有题目才有；知识点没有这个文件
├── 代码.cpp                 ← 用户本人写的实现（初赛笔记类知识点可以没有）
└── 思路.md 或 思路.txt      ← 用户的思路总结（通常是网页版 AI 总结出的笔记）
```

**处理规则**：

1. **文件名不固定**（`题目.md`/`题面.md`/`statement.*`、`代码.cpp`/`Untitled1.cpp`/`源.cpp` 都见过）——按**内容**判断哪个是题面、哪个是代码、哪个是思路（代码看 `#include`/`main`；题面看"题目描述/输入格式/数据范围"；思路看小标题与讲解口吻）。
2. **代码只搬运、不修改实现**：把用户的 `.cpp` 原样复制到约定路径；只有用户明确要求时才删除调试代码。
3. **题面必须忠实转述**，不许补写数据范围或样例；查不到来源就写 `来源未考证` / `-`。
4. **思路文件 → 规范章节**：把 `## 思路是怎么想出来的` 归入 `## 算法原理`，其余映射到「核心公式/复杂度分析/常见陷阱/对比与扩展」；小标题改成第 3.3 节的规范名。
5. 缺什么就问，不要猜（尤其是 `category` / `subcategory` / `id` / 题号）。

---

## 6. 门禁：`node tools/lint.js`

- 退出码：`0` = 无 error；`1` = 有 error；`2` = 脚本异常。**error 必须清零**，warning 建议修。
- 常用参数：`--quiet` 只打汇总、`--json` 结构化输出、`--report` 额外写 `tools/lint-report.md`、`node tools/lint.js content/xxx.md` 只查单个文件、`--only=E011,W028` 只跑指定规则。

**最常踩的规则（写内容前对照一遍）**：

| 规则 | 含义 | 怎么避免 |
|---|---|---|
| E007 | `codePath` 指向的文件不存在 | 先放 `.cpp`，再写 `codePath`（反斜杠、相对根） |
| E010 | 乱码（`代紅` 等） | 存 UTF-8；出现"洛谷"就写"洛谷" |
| W002 | 标题有草稿自述 | 用正式算法/题目名 |
| W007 / W008 | 缺复杂度 / 写了"未知" | 从代码层数数出来，如实填 |
| W010 | 标签超过 8 个 | 3–6 个最佳 |
| W013 | 图谱没有该知识点条目 | 在 `knowledgePoints` 补 `prerequisites`/`related` |
| W017 | 草稿式自我修正 | 改成结论式陈述 |
| W019 | 表格公式里有未转义 `|` | 用 `\lvert…\rvert` |
| W020 | 正文出现一级标题 | 正文从 `##` 开始（标题由 frontmatter 提供） |
| W022 / W023 | 缺必备章节 / 缺「适用场景」 | 按第 3.3 节补齐 |
| W024 | 题目缺「输入格式/输出格式/数据范围」 | 三节都写 |
| W027 | 题目缺 `oj` / `problemId` | 查不到写 `来源未考证` / `-` |
| W028 | 正文整段代码与 `.cpp` 不一致 | 改任一边就同步另一边 |
| W030 | 代码块疑似含调试/计时输出 | 删掉 `printf("耗时…")` 这类行 |
| W033 | `related` 单向 | 双向补齐 |
| W034 | 产物比内容旧 | 跑 `node build.js` |

---

## 7. 汇报格式（固定四段）

```
【改动】文件列表（每个文件一句话说明改了什么）
【验证】node tools/lint.js → error N / warning M；node build.js → X 知识点 / Y 题目（+ 我额外跑的验证）
【需要你本人做】例如：实现 xxx.cpp / 确认某处结论 / 提供题号
【待确认】我拿不准或做了判断的地方（写清"我按什么处理，不同意怎么回退"）
```

---

## 8. 环境事实（可能随时间变化，以实际为准）

| 项 | 值 |
|---|---|
| 项目根目录 | `C:\Users\penti\Desktop\算法 - 副本` |
| Node | 本机 PATH 里可能没有 `node`；Trae/终端里能跑就用，否则装 Node LTS 或用绝对路径 |
| `g++`（对拍/编译验证） | `C:\CSP_SIM\tools\mingw64\bin\g++.exe`（MinGW-w64 13.2.0） |
| PowerShell | 执行策略 Restricted：跑 `.ps1` 用 `tools\stress.cmd` 包装或加 `-ExecutionPolicy Bypass` |
| 脚本编码 | `.trae/skills/*.ps1` 之类保持 **ASCII + CRLF**，否则 Windows PowerShell 5.1 会读成乱码 |
| GitHub | 仓库 `https://github.com/World-Needs-Sunday/algorithm-knowledge-base.git`，分支 `master`，Pages 托管 |
| Pages 缓存 | `Cache-Control: max-age=600` —— push 后约 10 分钟线上才更新，别急着下结论 |

---

## 9. 绝对不要做的事

1. ❌ 生成/补全任何 `.cpp` 的实现（含暴力程序、造数据程序）。
2. ❌ 手改 `data.js` / `code_data.js` / `problems_data.js`。
3. ❌ 编造题号、样例、年份、出处、数据范围。
4. ❌ 没跑门禁就宣称"完成了"。
5. ❌ 顺手重构、批量改格式、改动与任务无关的文件。
6. ❌ 把用户的 `.cpp` 复制进正文后不核对一致性（W028 就是这么来的）。
7. ❌ 在正文里留调试输出、草稿痕迹、占位符（`TODO`/`待补充`）。
