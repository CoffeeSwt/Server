const fs = require("fs");
const path = require("path");

const factorArg = process.argv[2];
if (!factorArg) {
  console.error("用法: node index.js <倍数>");
  process.exit(1);
}
const factor = Number(factorArg);
if (!Number.isFinite(factor)) {
  console.error("倍数必须是数字");
  process.exit(1);
}

const xmlPath = path.resolve(
  "c:\\Server\\mpmissions\\empty.deerisle\\env\\zombie_territories.xml",
);
let content;
try {
  content = fs.readFileSync(xmlPath, "utf8");
} catch (e) {
  console.error("读取失败:", e.message);
  process.exit(1);
}

let changedCount = 0;
const replaced = content.replace(
  /(smin|smax|dmin|dmax)="([^"]+)"/g,
  (m, key, val) => {
    const num = Number(val);
    if (!Number.isFinite(num)) return m;
    const newVal = Math.round(num * factor);
    if (String(newVal) !== String(val)) changedCount++;
    return `${key}="${newVal}"`;
  },
);

if (replaced !== content) {
  try {
    fs.copyFileSync(xmlPath, xmlPath + ".bak");
  } catch (_) {}
  try {
    fs.writeFileSync(xmlPath, replaced, "utf8");
    console.log(`已更新文件: ${xmlPath}`);
    console.log(`修改属性次数: ${changedCount}`);
  } catch (e) {
    console.error("写入失败:", e.message);
    process.exit(1);
  }
} else {
  console.log("未发现可修改的属性或倍数未产生变化");
}
