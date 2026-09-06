# 项目框架与操作指南

> **用途**：本文件是项目的结构地图和操作手册。每次需要添加/修改/删除知识点时，直接查阅本文件即可，无需重新遍历整个项目。

---

## 1. 项目整体结构

```
算法 - 副本/
├── index.html          # 前端页面（单文件，含 CSS + JS）
├── build.js            # 构建脚本（扫描 content/ + problems/ + knowledge_graph.json 生成 data.js + problems_data.js + code_data.js）
├── knowledge_graph.json # 知识图谱（分类描述 + 前置/相关知识点关系）
├── data.js             # [自动生成] 知识点元数据 + Markdown 正文 + 知识图谱数据（勿手动编辑）
├── problems_data.js   # [自动生成] 题目专辑数据（勿手动编辑）
├── code_data.js        # [自动生成] C++ 代码内容（知识点 + 题目共用，勿手动编辑）
├── content/            # 知识点 Markdown 源文件（每篇一个 .md）
├── problems/           # 题目专辑 Markdown 源文件（每题一个 .md）
├── 题目专辑/            # 题目 C++ 代码目录（按分类/题名 组织）
├── vendor/             # 本地前端依赖（KaTeX、marked、highlight.js）
│   ├── katex.min.js / katex.min.css / auto-render.min.js
│   ├── marked.min.js
│   ├── highlight.min.js / github-dark.min.css
│   └── fonts/
├── 动态规划/            # C++ 代码目录（按分类/子分类/具体算法 组织）
├── 数据结构/
├── 图论/
├── 数学/
├── 字符串/
├── 基础算法(有价值的题目集)/
└── 输入,输出/
```

**核心数据流**：
- 知识点：`content/*.md` + `各分类目录/*.cpp` + `knowledge_graph.json` → `node build.js` → `data.js` + `code_data.js`
- 题目专辑：`problems/*.md` + `题目专辑/*.cpp` → `node build.js` → `problems_data.js`（代码合并到 `code_data.js`）
- `index.html` 加载 `data.js` + `problems_data.js` + `code_data.js` 渲染

### data.js 输出结构

| 变量名 | 内容 | 来源 |
|---|---|---|
| `KNOWLEDGE_DATA` | 知识点数组（含 id/title/category/content/prerequisites/related 等） | content/*.md + knowledge_graph.json |
| `CATEGORY_META` | 大类元数据（nature/description/problemDomain） | knowledge_graph.json |
| `SUBCATEGORY_META` | 子分类描述（key: "大类\|子分类"） | knowledge_graph.json |
| `SUBSUBCATEGORY_META` | 小分类描述（key: "大类\|子分类\|小分类"） | knowledge_graph.json |

### problems_data.js 输出结构

| 变量名 | 内容 | 来源 |
|---|---|---|
| `PROBLEMS_DATA` | 题目数组（含 id/title/oj/problemId/difficulty/category/tags/content 等） | problems/*.md |

---

## 2. 知识点分类体系

当前已有 81 个知识点，分 8 个大类：

| 大类 (category) | 子分类 (subcategory) | 知识点 ID |
|---|---|---|
| 基础算法 | 贪心 | `basic-interval-scheduling`, `basic-interval-merge` |
| 基础算法 | 双指针 | `basic-two-pointer-cycle` |
| 基础算法 | 分治 | `basic-divide-conquer-merge-inversion`, `basic-quickselect-kth` |
| 基础算法 | 数位提升 | `basic-digit-power-cycle-self` |
| 基础算法 | 排序 | `basic-radix-sort-k`（k进制基数排序） |
| 动态规划 | 背包问题 | 01背包: `dp-01bag-standard`, `dp-01bag-2d`, `dp-01bag-record`, `dp-count-01bag`; 完全背包: `dp-complete-bag`; 多重背包: `dp-multi-bag-binary`, `dp-multi-bag-monotone`; 分组背包: `dp-group-bag` |
| 动态规划 | 线性DP | LCS: `dp-linear-lcs-length`, `dp-linear-lcs-output`, `dp-linear-lcs-substring`; LIS: `dp-linear-lis-binary`, `dp-linear-lis-bit-count`; 经典模型: `dp-linear-edit-distance`, `dp-linear-maxsub`, `dp-linear-triangle` |
| 动态规划 | 树形DP | `dp-tree-01bag`, `dp-tree-dance` |
| 动态规划 | 状态压缩DP | 路径问题: `dp-bitmask-hamilton` |
| 数据结构 | 树状数组 | `ds-bit-point-range`, `ds-bit-range-point` |
| 数据结构 | 分块 | `ds-block-decomposition` |
| 数据结构 | 莫队 | `ds-mo-team` |
| 数据结构 | 单调队列 | `ds-monoqueue-1d`, `ds-monoqueue-2d` |
| 数据结构 | 前缀和与差分 | 一维: `ds-prefix-sum-1d`, `ds-diff-1d`; 二维: `ds-prefix-sum-2d`, `ds-diff-2d`; 树上: `ds-prefix-sum-tree`, `ds-diff-tree` |
| 数据结构 | 可持久化线段树 | `ds-persistent-segtree-3919`, `ds-persistent-segtree-template` |
| 数据结构 | 线段树 | `ds-segtree-basic`, `ds-segtree-discretize`, `ds-segtree-mul` |
| 数据结构 | 平衡树 | `ds-splay` |
| 数据结构 | 堆 | `ds-dual-heap` |
| 数据结构 | 栈 | `ds-stack-prefix-max`（前缀最值）, `ds-monostack-histogram`（单调栈） |
| 数据结构 | ST表 | `ds-st-table-1`, `ds-st-table-2` |
| 数据结构 | 并查集 | 基础: `ds-uf-basic`, `ds-uf-rank`; 带权: `ds-uf-weighted-old`, `ds-uf-weighted-food`; 扩展域: `ds-uf-extended-domain` |
| 图论 | 最短路 | 单源最短路: `graph-dijkstra-heap`, `graph-bellman-ford`, `graph-spfa`; 全源最短路: `graph-floyd`, `graph-johnson`; DAG最短路: `graph-dag-shortest-path` |
| 图论 | 最小生成树 | `graph-kruskal`, `graph-prim` |
| 图论 | 最近公共祖先 | `graph-lca-doubling`, `graph-lca-tarjan`, `graph-lca-hld` |
| 图论 | 拓扑排序 | `graph-topo-sort` |
| 图论 | 树链剖分 | `graph-hld-segtree` |
| 图论 | 割点与桥 | `graph-tarjan-cut-vertex`（Tarjan求割点） |
| 图论 | 强连通分量 | `graph-tarjan-scc`（Tarjan求SCC）, `graph-scc-condensation`（SCC缩点） |
| 输入输出 | IO优化 | `io-fast-io` |
| 数学 | 数论计数 | `math-lcm-pair-count` |
| 数学 | 矩阵 | `math-matrix-multiplication`（矩阵乘法）, `math-matrix-quick-pow`（矩阵快速幂）, `math-matrix-linear-recurrence`（矩阵加速线性递推） |
| 数学 | 快速幂 | `math-quick-pow-basic`, `math-quick-pow-highprecision` |
| 字符串 | 字符串哈希 | `str-hash` |
| 字符串 | 最小表示法 | `str-minimal-representation` |
| 字符串 | 字符串匹配 | `str-kmp`（KMP算法）, `str-sunday`（Sunday算法） |

### ID 命名规范

格式：`{分类前缀}-{子分类缩写}-{具体算法名}`

| 分类前缀 | 对应大类 | 示例 |
|---|---|---|
| `basic-` | 基础算法 | `basic-palindrome-stack` |
| `dp-` | 动态规划 | `dp-01bag-standard` |
| `ds-` | 数据结构 | `ds-segtree-basic` |
| `graph-` | 图论 | `graph-johnson` |
| `io-` | 输入输出 | `io-fast-io` |
| `math-` | 数学 | `math-quick-pow-basic` |
| `str-` | 字符串 | `str-hash` |

同一算法有多个版本时，用后缀区分：`-standard`、`-2d`、`-binary`、`-monotone`、`-template`、`-self`（自己写的）、`-old`（旧版）等。

---

## 3. 文件格式规范

### 3.1 Markdown 知识点文件 (`content/*.md`)

**文件名**：`{id}.md`，如 `graph-johnson.md`

**文件结构**：

```markdown
---
id: graph-johnson
title: 'Johnson 全源最短路（负权图）'
category: 图论
subcategory: 最短路
tags: ["图论", "最短路", "Johnson", "全源最短路", "负权边"]
timeComplexity: 'O(nm log n)'
spaceComplexity: 'O(n^2 + n + m)'
codePath: '图论\最短路\Johnson\Untitled1.cpp'
---

## 算法原理
（正文内容...）

## 后续各章节...
```

### 3.2 Frontmatter 字段说明

| 字段 | 必填 | 说明 | 示例 |
|---|---|---|---|
| `id` | 是 | 知识点唯一标识，与文件名（去 .md）一致 | `graph-johnson` |
| `title` | 是 | 显示标题，用单引号包裹 | `'Johnson 全源最短路（负权图）'` |
| `category` | 是 | 大类（见分类体系表） | `图论` |
| `subcategory` | 是 | 子分类 | `最短路` |
| `subSubcategory` | 否 | 小分类（子分类内的进一步分组，如"栈"下设"前缀最值""单调栈"） | `单调栈` |
| `tags` | 否 | 标签数组 | `["图论", "最短路", "Johnson"]` |
| `timeComplexity` | 否 | 时间复杂度 | `'O(nm log n)'` |
| `spaceComplexity` | 否 | 空间复杂度 | `'O(n^2 + n + m)'` |
| `codePath` | 是 | C++ 代码文件相对路径（反斜杠） | `'图论\最短路\Johnson\Untitled1.cpp'` |

> **前置/相关知识点**：`prerequisites` 和 `related` 不在 .md frontmatter 中定义，而是在 `knowledge_graph.json` 中统一管理。build.js 读取后自动合并到 `KNOWLEDGE_DATA`。

### 3.3 正文章节规范

**数据结构类知识点**（8 个章节）：

1. `## 算法原理` — 核心思想、数据结构定义
2. `## 核心公式/状态定义` — 数学公式（LaTeX）+ 结构定义（偏数学型数据结构用）
   或 `## 数据结构图示` — ASCII 图示例，展示结构布局（偏结构型数据结构用）
3. `## 数据结构图示` — ASCII 图示例，操作过程示意图（若第 2 章是公式，则本章为图示）
   或 `## 核心操作详解` — 逐个操作分小节讲解（若第 2 章是图示，则本章为操作）
4. `## 逐行代码解析` — 代码分块 + 逐行注释
5. `## 复杂度分析` — 时间/空间复杂度推导
6. `## 适用场景` — 适用与不适用场景
7. `## 常见陷阱与注意事项` — 易错点列表
8. `## 对比与扩展` — 与同类数据结构/算法的表格对比

> **说明**：数据结构类必须包含「数据结构图示」章节。偏结构型（如线段树、并查集）用「图示 + 核心操作」组合；偏数学推导型（如前缀和、差分、ST表）用「核心公式 + 数据结构图示」组合。两种均为 8 章。

**算法类知识点**（7 个章节）：

1. `## 算法原理` — 核心思想、算法框架
2. `## 核心公式/状态定义与转移方程` — 数学公式（LaTeX）+ 状态转移
3. `## 逐行代码解析` — 代码分块 + 逐行注释
4. `## 复杂度分析` — 时间/空间复杂度推导
5. `## 适用场景` — 适用与不适用场景
6. `## 常见陷阱与注意事项` — 易错点列表
7. `## 对比与扩展` — 与同类算法的表格对比

### 3.4 C++ 代码文件

- **文件名**：统一用 `Untitled1.cpp`（Dev-C++ 默认名，用户习惯）
- **放置路径**：`{大类}\{子分类}\{具体算法名}\Untitled1.cpp`
- **编码**：UTF-8（build.js 用 Node.js 自动检测 UTF-8/GBK 编码读取）
- **示例路径**：`图论\最短路\Johnson\Untitled1.cpp`

### 3.5 codePath 路径映射

`codePath`（frontmatter）→ 项目根目录下的相对路径：

| codePath | 实际文件位置 |
|---|---|
| `图论\最短路\Johnson\Untitled1.cpp` | `算法 - 副本\图论\最短路\Johnson\Untitled1.cpp` |
| `数据结构\普通线段树\基础模板\Untitled1.cpp` | `算法 - 副本\数据结构\普通线段树\基础模板\Untitled1.cpp` |
| `动态规划\背包问题模板\01背包\Untitled1.cpp` | `算法 - 副本\动态规划\背包问题模板\01背包\Untitled1.cpp` |

目录结构不需要与 category/subcategory 完全对应，但通常保持一致以便维护。

### 3.6 知识图谱文件 (`knowledge_graph.json`)

定义分类描述、子分类描述和知识点间的前置/相关关系。build.js 读取此文件并合并到 data.js。

**结构**：

```json
{
  "categories": {
    "数据结构": {
      "nature": "数据组织与查询",
      "description": "通过组织数据存储方式实现...",
      "problemDomain": "区间查询、单点修改..."
    }
  },
  "subcategories": {
    "数据结构|并查集": {
      "description": "维护不相交集合的数据结构..."
    }
  },
  "subSubcategories": {
    "数据结构|并查集|基础": {
      "description": "基础并查集，仅路径压缩或按秩合并..."
    },
    "数据结构|并查集|带权": {
      "description": "带权并查集，在路径压缩时维护权值关系..."
    }
  },
  "knowledgePoints": {
    "ds-uf-basic": {
      "prerequisites": [],
      "related": ["ds-uf-rank", "graph-kruskal"]
    }
  }
}
```

**字段说明**：

| 字段 | 位置 | 说明 |
|---|---|---|
| `nature` | categories | 分类性质标签（如"最优化方法"） |
| `description` | categories/subcategories/subSubcategories | 分类/子分类/小分类的详细描述 |
| `problemDomain` | categories | 该分类处理的问题领域 |
| `prerequisites` | knowledgePoints | 前置知识点 ID 数组（学习本知识点前应掌握的内容） |
| `related` | knowledgePoints | 相关知识点 ID 数组（延伸阅读） |

**维护规则**：
- 添加新知识点时，在 `knowledgePoints` 中添加对应条目
- 新增子分类时，在 `subcategories` 中添加描述（key: `"大类|子分类"`）
- 新增小分类时，在 `subSubcategories` 中添加描述（key: `"大类|子分类|小分类"`），同时在对应 .md 文件的 frontmatter 中设置 `subSubcategory` 字段
- 新增大类时，在 `categories` 中添加描述
- 前置/相关关系使用知识点 ID 引用，build.js 会在构建时自动关联标题

### 3.7 题目专辑 Markdown 文件 (`problems/*.md`)

**文件名**：`{id}.md`，如 `prob-luogu-p1001.md`

**文件结构**：

```markdown
---
id: prob-luogu-p1001
title: 'A+B Problem'
oj: '洛谷'
problemId: 'P1001'
difficulty: '简单'
category: '基础题'
tags: ["输入输出", "基础"]
timeComplexity: 'O(1)'
spaceComplexity: 'O(1)'
codePath: '题目专辑\基础题\A+BProblem\Untitled1.cpp'
---

## 题目描述
（题目正文...）

## 输入格式
...

## 输出格式
...

## 样例

### 样例输入
```
1 2
```

### 样例输出
```
3
```

## 解题思路
（总结细化的思路...）

## 逐行代码解析
...

## 复杂度分析
...

## 常见陷阱与注意事项
...
```

### 题目 Frontmatter 字段说明

| 字段 | 必填 | 说明 | 示例 |
|---|---|---|---|
| `id` | 是 | 题目唯一标识，与文件名（去 .md）一致 | `prob-luogu-p1001` |
| `title` | 是 | 题目标题 | `'A+B Problem'` |
| `oj` | 否 | 来源 OJ 平台 | `'洛谷'`、`'Codeforces'`、`'LeetCode'` |
| `problemId` | 否 | 题目编号 | `'P1001'`、`'CF1234A'` |
| `difficulty` | 否 | 难度（简单/中等/困难） | `'简单'` |
| `category` | 是 | 题目分类（用于侧边栏分组） | `'基础题'`、`'贪心'`、`'DP'` |
| `tags` | 否 | 标签数组 | `["输入输出", "基础"]` |
| `timeComplexity` | 否 | 时间复杂度 | `'O(n log n)'` |
| `spaceComplexity` | 否 | 空间复杂度 | `'O(n)'` |
| `codePath` | 否 | C++ 代码文件相对路径（反斜杠） | `'题目专辑\贪心\xxx\Untitled1.cpp'` |

### 题目正文章节规范

题目正文通常包含以下章节（可根据题目调整）：

1. `## 题目描述` — 题目原文描述
2. `## 输入格式` — 输入说明
3. `## 输出格式` — 输出说明
4. `## 样例` — 样例输入/输出
5. `## 解题思路` — 总结细化的思路分析（核心部分）
6. `## 逐行代码解析` — 代码分块 + 逐行注释
7. `## 复杂度分析` — 时间/空间复杂度
8. `## 常见陷阱与注意事项` — 易错点

> **说明**：题目文件不需要 knowledge_graph.json 条目，build.js 直接从 frontmatter 读取所有信息。

---

## 4. 前端渲染机制（index.html）

### 4.1 技术栈

| 库 | 用途 | 来源 |
|---|---|---|
| KaTeX | 渲染 LaTeX 数学公式 | `vendor/katex.min.js` + `vendor/katex.min.css` |
| marked.js | Markdown → HTML | `vendor/marked.min.js` |
| highlight.js | 代码高亮 | `vendor/highlight.min.js` + `vendor/github-dark.min.css` |
| auto-render | 自动扫描 `$`/`$$` 并渲染 | `vendor/auto-render.min.js` |

### 4.2 三步法 LaTeX 保护（重要！）

marked.js 会破坏 LaTeX 语法（转义反斜杠、拆分 `$` 分隔符）。index.html 用**提取-解析-恢复**三步法保护公式：

1. **提取块级公式** `$$...$$` → 替换为占位符 `@@KATEX0@@`、`@@KATEX1@@`...
2. **提取行内公式** `$...$` → 同样替换为占位符
3. **解析 Markdown**（占位符是纯文本，marked.js 不会破坏）
4. **恢复公式**：将占位符替换为 KaTeX 渲染后的 HTML

### 4.3 页面布局

- **左侧栏**：暗色侧边栏，按 category → subcategory → 知识点 三级树展示，带搜索框。点击分类名或子分类名可跳转到分类概览页
- **分类概览页**：展示分类性质、描述、问题领域，以及所有子分类的描述和知识点卡片
- **知识点详情页**：显示 title → 标签/复杂度徽章 → 前置知识点（可点击跳转）→ Markdown 正文 → 代码区块 → 相关知识点（可点击跳转）→ 前后导航按钮
- **总目录页**：全局概览所有知识点，按分类和子分类组织。分类卡片默认收起，点击展开/收起知识点列表，右侧"查看详情 →"跳转分类概览页
- **主题**：支持亮/暗主题切换（localStorage 存储）
- **响应式**：侧边栏可折叠

---

## 5. 操作手册

### 5.1 添加新知识点（完整步骤）

**前置确认**：
- 确定 category（大类）和 subcategory（子分类）
- 确定 id（按命名规范）
- 确认是否已有同类知识点可参考

**Step 1：创建 C++ 代码文件**

```
路径：{项目根}\{大类}\{子分类}\{具体算法名}\Untitled1.cpp
```

- 如果目录不存在，先创建
- 代码文件编码推荐 UTF-8，GBK 也可（build.js 自动检测编码）

**Step 2：创建 Markdown 知识点文件**

```
路径：{项目根}\content\{id}.md
```

按 3.1 节格式编写 frontmatter + 正文。**注意**：
- `codePath` 使用反斜杠 `\`，路径与 Step 1 的实际路径一致
- 数据结构类写 8 个章节，算法类写 7 个章节（见 3.3 节）
- 参考已有同类知识点的 .md 文件结构和深度

**Step 3：更新知识图谱**

在 `knowledge_graph.json` 中：
1. 在 `knowledgePoints` 中添加新知识点的 `prerequisites` 和 `related`（见 3.6 节）
2. 如果新增了子分类，在 `subcategories` 中添加描述
3. 如果新增了大类，在 `categories` 中添加描述

**Step 4：运行构建**

```bash
cd "c:\Users\penti\Desktop\算法 - 副本"
node build.js
```

构建脚本会：
1. 扫描 `content/` 下所有 `.md` 文件
2. 解析 frontmatter 获取元数据
3. 读取每个 `codePath` 指向的 `.cpp` 文件（自动检测 UTF-8/GBK 编码）
4. 读取 `knowledge_graph.json` 合并前置/相关知识点和分类描述
5. 生成 `data.js`（知识点数据 + 知识图谱）和 `code_data.js`（代码内容）
6. 输出统计信息和警告

**Step 5：验证**

- 构建输出中确认新知识点出现在列表中且无警告
- 浏览器打开 `index.html`，Ctrl+F5 强制刷新
- 在左侧栏找到新知识点，检查内容渲染（公式、代码高亮）
- **重点检查 LaTeX 公式**：确认所有 `$...$` 和 `$$...$$` 公式正确渲染，无红色报错；特别检查 `\text{}` 内是否有未转义的下划线 `_`（须写成 `\_`，见 6.1 节）；同时检查反引号代码段内是否混入了 LaTeX 语法（如 `` `fac[i] = 2^i \bmod \text{mod}` ``），应将代码引用与数学公式分开（见 6.1 节）
- **检查示例计算**：确认数据结构图示和代码解析中的数值计算正确，不得包含推导过程中的错误尝试或自我纠正步骤；如发现推导有误，应先验证正确结果，再只写入正确的最终版本
- 检查前置知识点和相关知识点链接是否正确跳转

### 5.2 修改已有知识点

#### 5.2.1 修改正文/元数据

1. 编辑 `content/{id}.md`
2. 运行 `node build.js`
3. Ctrl+F5 刷新

#### 5.2.2 修改代码文件（仅改 .cpp，不改文档）

当代码做了小幅修改（如修 bug、改类型、优化循环顺序），但文档中的讲解仍然适用时：

1. **直接编辑** 对应的 `.cpp` 文件
2. 运行 `node build.js`（codePath 不变则无需改 .md）
3. Ctrl+F5 刷新

#### 5.2.3 修改代码文件（同步更新文档）

当代码修改影响了文档中的代码片段或讲解内容时，必须同步更新文档：

1. **更新 .cpp 文件** — 替换为最新代码
2. **更新 content/{id}.md 中的代码块** — 找到文档中所有引用旧代码的 ```cpp 代码段，逐个替换为新代码
3. **更新文档中的讲解文字** — 如果代码改动涉及函数签名、循环顺序、数据类型等，文档中的逐行解析、要点表、常见陷阱等文字描述也需同步修改
4. **检查文档中的旧注意事项** — 如果文档中有"本代码用 XXX，应改为 YYY"之类的提醒，改完代码后该提醒已过时，需删除或更新为"已使用 YYY"
5. 运行 `node build.js`
6. Ctrl+F5 刷新

**常见场景**：

| 改动类型 | 需更新的文档内容 |
|---------|----------------|
| 变量类型变更（如 int→long long） | 代码块 + 陷阱条目 + 注意事项 |
| 循环顺序变更（如 i-j-k→i-k-j） | 代码块 + 循环变量表 + 细节说明 |
| 函数签名变更（加 const&、参数化） | 代码块 + 要点表 + 规范性建议 |
| 新增/删除辅助函数 | 代码块 + 逐行解析章节 |
| 算法逻辑变更 | 几乎全部章节需重写 |

#### 5.2.4 修改分类/子分类

1. 编辑 .md 的 frontmatter 中 `category`/`subcategory`
2. 运行 `node build.js`
3. Ctrl+F5 刷新（侧边栏树会自动重组）

### 5.3 删除知识点

1. 删除 `content/{id}.md` 文件
2. （可选）删除对应的 `.cpp` 代码目录
3. 运行 `node build.js`
4. Ctrl+F5 刷新

### 5.4 添加新题目到题目专辑

**Step 1：创建 C++ 代码文件**

```
路径：{项目根}\题目专辑\{分类}\{题名}\Untitled1.cpp
```

- 如果目录不存在，先创建
- 代码文件编码推荐 UTF-8，GBK 也可

**Step 2：创建题目 Markdown 文件**

```
路径：{项目根}\problems\{id}.md
```

按 3.7 节格式编写 frontmatter + 正文。注意：
- `codePath` 使用反斜杠 `\`，路径与 Step 1 的实际路径一致
- `category` 决定题目在侧边栏中的分组
- `difficulty` 填写"简单"/"中等"/"困难"（影响显示颜色）
- 正文至少包含「题目描述」和「解题思路」

**Step 3：运行构建**

```bash
node build.js
```

构建脚本会自动扫描 `problems/` 目录，生成 `problems_data.js`。

**Step 4：验证**

- 浏览器打开 `index.html`，Ctrl+F5 强制刷新
- 点击侧边栏顶部「📝 题目专辑」切换到题目模式
- 在侧边栏找到新题目，检查内容渲染
- 检查 LaTeX 公式和代码高亮是否正常

> **说明**：题目专辑不需要修改 `knowledge_graph.json`，所有信息从 .md 的 frontmatter 读取。

---

## 6. 常见陷阱与注意事项

### 6.1 KaTeX 公式转义（高频问题）

| 问题 | 原因 | 解决 |
|---|---|---|
| `\text{__builtin_clz}` 报错 | KaTeX 将 `__` 解析为下标 | 写成 `\text{\_builtin\_clz}` 或用 `\text{\_\_builtin\_clz}` |
| `\text{}` 内含下划线 | `_` 是 KaTeX 下标操作符 | 所有 `\text{}` 内的下划线必须转义为 `\_` |
| `\text{\_}` 反斜杠被吞 | marked.js 可能剥离 `\_` 中的反斜杠 | **最佳方案：避免在 `\text{}` 内使用下划线**，改用其他符号（如用 `0` 代替占位下划线） |
| 公式中 `\le` 不显示 | marked.js 转义了反斜杠 | 三步法已处理，确保公式用 `$...$` 或 `$$...$$` 包裹 |
| 行内公式被 marked.js 破坏 | `$` 被 marked.js 当普通文本 | 三步法提取保护，确保 `$` 两侧无多余空格干扰匹配 |
| 整个公式显示为原始文本 | 公式内有 KaTeX 无法解析的内容 | 检查 `\text{}` 内特殊字符，一个错误会导致整个公式不渲染 |

**核心原则**：
- `\text{}` 块内所有 `_` 必须写成 `\_`
- **若 `\_` 仍不生效，直接避免使用下划线**，用替代符号（如 `0`、`\ast`、`\cdot`）

### 6.2 代码文件编码

- build.js 使用 Node.js 内置的 `TextDecoder('gbk')` 自动检测编码读取 `.cpp` 文件
- 先检测字节流是否为合法 UTF-8，是则按 UTF-8 读取，否则按 GBK 解码
- 不再依赖 PowerShell，无执行策略限制
- 推荐统一保存为 UTF-8 编码，避免编码混乱

### 6.3 路径注意事项

- `codePath` 使用**反斜杠** `\`（Windows 风格）
- 路径不需要以 `\` 开头，是相对项目根目录的路径
- build.js 用 `path.join(ROOT, codePath)` 拼接完整路径读取代码文件
- 如果 codePath 路径与实际文件路径不匹配，build.js 会输出 `// 代码文件未找到: xxx`

### 6.4 构建后必须刷新

- `data.js` 和 `code_data.js` 是静态文件，浏览器会缓存
- 修改后必须 **Ctrl+F5**（强制刷新）才能看到更新
- 普通刷新（F5）可能加载缓存的旧版本

### 6.5 data.js / code_data.js 勿手动编辑

- 这两个文件由 build.js 自动生成
- 手动修改会在下次构建时被覆盖
- 所有内容修改应在 `content/*.md` 和 `.cpp` 文件中进行

---

## 7. build.js 关键逻辑速查

| 功能 | 实现方式 |
|---|---|
| 扫描 .md 文件 | `fs.readdirSync(CONTENT_DIR).filter(f => f.endsWith('.md')).sort()` |
| 解析 frontmatter | 正则匹配 `^---\r?\n([\s\S]*?)\r?\n---\r?\n([\s\S]*)$` |
| 解析 YAML 字段 | 逐行正则 `^(\w+):\s*(.*)$`，数组格式 `[a, b, c]` 单独处理 |
| 读取代码文件 | Node.js `TextDecoder` 自动检测 UTF-8/GBK 编码读取 |
| 读取知识图谱 | `JSON.parse(fs.readFileSync(GRAPH_FILE, 'utf8'))` |
| 输出 data.js | `KNOWLEDGE_DATA` + `CATEGORY_META` + `SUBCATEGORY_META` 三个变量 |
| 输出 code_data.js | `const CODE_DATA = ${JSON.stringify(codeData, null, 4)}` |
| 必填字段校验 | `['id', 'title', 'category', 'subcategory', 'codePath']` |

---

## 8. 快速参考卡

```
添加知识点：
  1. 写代码   → {大类}\{子分类}\{算法名}\Untitled1.cpp
  2. 写文档   → content\{id}.md（frontmatter + 章节）
  3. 更新图谱 → knowledge_graph.json（prerequisites + related）
  4. 构建     → node build.js
  5. 验证     → 浏览器 Ctrl+F5

添加题目：
  1. 写代码   → 题目专辑\{分类}\{题名}\Untitled1.cpp
  2. 写文档   → problems\{id}.md（frontmatter + 题目描述 + 解题思路）
  3. 构建     → node build.js
  4. 验证     → 浏览器 Ctrl+F5（切换到「📝 题目专辑」模式）

修改知识点：
  1. 改 .md 或 .cpp
  2. 改 knowledge_graph.json（如果前置/相关有变化）
  3. node build.js
  4. Ctrl+F5

关键路径：
  知识点文档 → content\{id}.md
  题目文档   → problems\{id}.md
  知识点代码 → {大类}\{子分类}\{算法名}\Untitled1.cpp
  题目代码   → 题目专辑\{分类}\{题名}\Untitled1.cpp
  知识图谱   → knowledge_graph.json
  构建脚本   → build.js
  前端页面   → index.html
  生成产物   → data.js + problems_data.js + code_data.js
  依赖库     → vendor\

LaTeX 注意：
  \text{} 内的下划线必须转义为 \_
  公式用 $...$ 或 $$...$$ 包裹
  反引号代码段内不要混入 LaTeX 语法（如 \bmod、\text{}），marked.js 会将反引号内容视为纯文本不渲染公式，且可能干扰 $ 分隔符的解析
  正确做法：将代码引用与数学公式分开，如 `fac[i]`（即 $2^i \bmod \text{mod}$）
```
