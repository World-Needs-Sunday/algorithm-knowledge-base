# 项目规则 · 算法知识点知识库

> 本文件会被 Trae **自动注入每一次对话**，所以只放铁律与入口，不放长篇教程。
> 完整结构、字段表、章节规范、四个能力的逐步流程：
> - 铁律与入口 → [`AGENTS.md`](../../AGENTS.md)
> - 框架与操作手册 → [`PROJECT_FRAMEWORK.md`](../../PROJECT_FRAMEWORK.md)
> - 项目说明 → [`README.md`](../../README.md)
> - 四个能力技能 → `.trae/skills/add-knowledge`、`add-problem`、`edit-content`、`release-github`

## 0. 三条铁律（最高优先级）

1. **不代写解题代码。** 所有 `.cpp`（知识点示例、题目正解、暴力程序、造数据程序）由用户本人编写。
   - 可以：讲思路、给状态定义与转移方程、指出 bug 与修复方向、列边界清单、设计测试数据、写对拍脚本框架、review 代码、删除调试输出。
   - 不可以：生成或补全这些 `.cpp` 的实现；在正文里写出完整可提交代码。
   - 例外：用户明确说"这次破例"时才可以写，且回复第一行标注 `⚠️ 已按你的破例要求代写代码`。
2. **改完必须过门禁。** `node tools/lint.js`（error 必须 0）→ `node build.js`。
3. **内容源是唯一真相。** 只存在于 `content/*.md`、`problems/*.md`、`knowledge_graph.json`、各分类目录的 `.cpp`；
   `data.js` / `code_data.js` / `problems_data.js` 是产物，**永不手改**。

## 1. 四个能力（用户说什么 → 用哪个技能）

| 用户说什么 | 技能 |
|---|---|
| "这是新知识点 / 帮我入库" | `.trae/skills/add-knowledge/SKILL.md` |
| "这题加进题目专辑" | `.trae/skills/add-problem/SKILL.md` |
| "XX 写错了 / 复杂度不对 / 样例应改为…" | `.trae/skills/edit-content/SKILL.md` |
| "更新网站 / 上传 / 提交到 GitHub" | `.trae/skills/release-github/SKILL.md` |

## 2. 用户的输入习惯

用户丢**一个文件夹**过来：`题目.md`（只有题目才有）+ `代码.cpp` + `思路.md`。
文件名不固定，**按内容判断**角色；代码只搬运不改实现；题面忠实转述，不补数据范围/样例。

## 3. 常用命令

```bash
node tools/lint.js        # 内容门禁（error 必须 0）
node build.js             # 生成三个产物
node tools/new-kp.js --help
node tools/new-problem.js --help
```

## 4. 汇报格式（固定四段）

```
【改动】文件列表
【验证】node tools/lint.js → error N / warning M；node build.js → X 知识点 / Y 题目
【需要你本人做】例如：实现 xxx.cpp / 确认某处结论 / 提供题号
【待确认】我做了判断的地方（依据 + 回退方式）
```

## 5. 绝对不要做

- ❌ 生成/补全任何 `.cpp` 实现（含暴力、造数据）
- ❌ 手改三个产物
- ❌ 编造题号、样例、年份、出处、数据范围
- ❌ 没跑门禁就宣称完成
- ❌ 顺手重构、批量改格式、改动与任务无关的文件
