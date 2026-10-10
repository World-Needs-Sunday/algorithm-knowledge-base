---
name: "release-github"
description: "构建校验并上传 GitHub。当用户说「更新网站」「上传」「提交到 GitHub」「发布」时调用。流程：门禁 → 构建 → 运行时可访问性自检 → git 提交推送 → 线上验证 → 失败回滚。含首次发布需要补齐的文件清单（favicon.svg 等）。"
---

# 构建 + 上传 GitHub

> 仓库：`https://github.com/World-Needs-Sunday/algorithm-knowledge-base.git`，分支 `master`，GitHub Pages 托管。
> 线上地址：`https://world-needs-sunday.github.io/algorithm-knowledge-base/`

## 铁律

1. 推送前 **error 必须为 0**（`node tools/lint.js`）且**刚跑过 `build.js`**（否则线上是旧数据）。
2. 不代写 `.cpp`、不手改产物（见 `AGENTS.md`）。
3. 每次发布都留一条**可回滚**的记录（commit sha）。

## Step 1 · 门禁

```bash
node tools/lint.js            # error 必须 0
node tools/lint.js --quiet    # 只看汇总
```

有 error 就停下来先修——**不要带着 error 发布**。

## Step 2 · 构建产物

```bash
node build.js
```

输出例：`109 个知识点 / 20 个题目 / 128 个代码文件`。
若数量与你的预期不符（比如刚加了 1 条却没变），说明 frontmatter 有问题，回 Step 1 看警告。

## Step 3 · 运行时可访问性自检（发布前必做）

网页真正需要的只有这几个文件，**它们必须都在，且都被 git 跟踪**：

| 文件 | 作用 | 检查 |
|---|---|---|
| `index.html` | 页面本体 | `git ls-files --error-unmatch index.html` |
| `data.js` / `problems_data.js` / `code_data.js` | 三个数据产物 | 同上 |
| `vendor/**` | KaTeX / marked / highlight.js + 字体 | 同上 |
| `favicon.svg` | 站点图标 | ⚠️ **目前未跟踪，线上会 404** |

⚠️ **已知问题**：`favicon.svg` 还没入库，线上站点其实没有图标。首次发布时请一并补上：

```bash
git add favicon.svg .gitignore
```

要快速自检"哪些运行时文件没被跟踪"：

```bash
git -c core.quotepath=false ls-files --others --exclude-standard
```

**建议同时入库的维护件**（不入库则克隆下来无法再维护）：
`AGENTS.md`、`README.md`、`PROJECT_FRAMEWORK.md`、`tools/`、`templates/`、`docs/`、`.trae/`

> 这些目前都未跟踪。是否公开取决于你（体积很小，且不含隐私；`.ssh/` 已在 `.gitignore` 里，不会上传）。

## Step 4 · 本地预览（可选但推荐）

双击 `index.html` 或起本地服务：

```bash
npx serve .        # 或 python -m http.server
```

重点看：首页统计数字、随便点两条（公式 + 代码高亮）、题目专辑、标签筛选。

## Step 5 · 提交与推送

```bash
git status --short            # 确认改动范围符合预期（不要漏、不要多）
git add -A                    # 或指定文件：git add content/xxx.md problems/yyy.md
git commit -m "feat(知识点): 新增 区间DP·石子合并"
git push origin master
```

**commit message 约定**：

| 前缀 | 用于 |
|---|---|
| `feat(知识点):` / `feat(题目):` | 新增条目 |
| `fix(知识点):` / `fix(题目):` | 修正内容 |
| `docs:` | 说明/框架/技能文件 |
| `chore:` | 构建产物、配置、清理 |

> Windows 上 git 会提示 `LF will be replaced by CRLF`——**忽略即可**，不是错误。

## Step 6 · 线上验证

GitHub Pages 有 `Cache-Control: max-age=600`，**push 后约 10 分钟**线上才更新。别急着断言"没生效"。

```bash
# 约 10 分钟后比对线上与本地是否一致（大小一致即可初判）
curl -sI https://world-needs-sunday.github.io/algorithm-knowledge-base/data.js | findstr /i content-length
(Get-Item data.js).Length
```

或直接在浏览器打开线上地址 + **Ctrl+F5**，看新条目是否出现、公式是否正常。

## Step 7 · 回滚（出问题时）

```bash
git log --oneline -5                       # 找到上一个正常版本的 sha
git revert <坏提交的sha>                    # 生成一个"反做"提交（最安全）
git push origin master
```

紧急且只影响自己时，也可以：

```bash
git reset --hard <正常版本的sha>
git push --force-with-lease origin master  # 谨慎：会改写远端历史
```

## 验收清单

- [ ] `node tools/lint.js` error = 0
- [ ] `node build.js` 已跑，产物数量符合预期
- [ ] 运行时文件（含 `favicon.svg`）都已 `git add`
- [ ] `git status` 里没有意外文件（临时文件、`_*.html`、`*.exe`）
- [ ] commit message 用了约定前缀
- [ ] push 成功（`git status` 显示与 origin 同步）
- [ ] 10 分钟后线上能看到本次改动

## 常见坑

| 坑 | 后果 | 处理 |
|---|---|---|
| 忘了 `build.js` 就 push | 线上内容还是旧的（产物没更新） | 每次 push 前必跑 |
| 忘了 `favicon.svg` | 线上图标 404 | `git add favicon.svg` |
| 只 `git add` 了 `.md`，没加产物 | 线上看不到新条目 | `git status` 复核，或 `git add -A` |
| push 后立刻检查线上 | 误判"没成功" | 等约 10 分钟 |
| 用 `git push --force` 前没看历史 | 丢提交 | 优先 `git revert` |
| 把 `_fail.in` / `*.exe` / 临时文件提交进去 | 仓库变脏 | `.gitignore` 已覆盖，仍要 `git status` 复核 |

## 汇报模板

```
【改动】本次提交：content/xxx.md（新增）、data.js 等产物（重建）；commit <sha>
【验证】node tools/lint.js → error 0 / warning N；node build.js → 109 知识点 / 20 题目；已 push 到 master
【需要你本人做】约 10 分钟后打开线上地址 Ctrl+F5 确认；确认是否把 tools/docs/.trae 一并入库
【待确认】favicon.svg 已补入库（此前未跟踪导致线上无图标）
```
