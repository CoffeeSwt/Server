const fs = require("fs");
const path = require("path");

const missionDir = __dirname;
const initPath = path.join(missionDir, "init.c");
const bunkerLayoutsDir = path.join(missionDir, "bunkerlayouts");
const includePattern = /^([ \t]*)#include\s+"([^"\r\n]*bunkerlayouts(?:\\\\|\/)[^"\r\n]+\.c)"[ \t]*$/gm;

function normalizeIncludePath(rawPath) {
	return rawPath.replace(/\\\\/g, "\\").replace(/\//g, "\\");
}

function readInlineContent(filePath) {
	return fs.readFileSync(filePath, "utf8").replace(/^\uFEFF/, "").trimEnd();
}

function getBunkerLayoutFiles() {
	return fs
		.readdirSync(bunkerLayoutsDir, { withFileTypes: true })
		.filter((entry) => entry.isFile() && entry.name.toLowerCase().endsWith(".c"))
		.map((entry) => entry.name)
		.sort((a, b) => a.localeCompare(b, undefined, { sensitivity: "base" }));
}

function main() {
	if (!fs.existsSync(initPath)) {
		throw new Error(`找不到 init.c: ${initPath}`);
	}

	if (!fs.existsSync(bunkerLayoutsDir)) {
		throw new Error(`找不到 bunkerlayouts 目录: ${bunkerLayoutsDir}`);
	}

	const original = fs.readFileSync(initPath, "utf8");
	const inlinedFiles = new Set();
	let replacedCount = 0;

	const output = original.replace(includePattern, (_, __, includePath) => {
		const normalizedPath = normalizeIncludePath(includePath);
		const fileName = path.basename(normalizedPath);
		const absoluteFilePath = path.join(bunkerLayoutsDir, fileName);

		if (!fs.existsSync(absoluteFilePath)) {
			throw new Error(`include 指向的文件不存在: ${absoluteFilePath}`);
		}

		inlinedFiles.add(fileName);
		replacedCount += 1;
		return readInlineContent(absoluteFilePath);
	});

	if (replacedCount === 0) {
		throw new Error("未找到任何可替换的 bunkerlayouts include。");
	}

	if (includePattern.test(output)) {
		throw new Error("替换完成后仍然存在 bunkerlayouts include，请检查 init.c 的 include 写法。");
	}

	fs.writeFileSync(initPath, output, "utf8");

	const unreferencedFiles = getBunkerLayoutFiles().filter((fileName) => !inlinedFiles.has(fileName));

	console.log(`已内联 ${replacedCount} 个 bunkerlayouts include 到 init.c`);

	if (unreferencedFiles.length > 0) {
		console.log("以下 .c 文件存在于 bunkerlayouts 中，但没有出现在 init.c 的 include 中：");
		for (const fileName of unreferencedFiles) {
			console.log(`- ${fileName}`);
		}
	}
}

main();
