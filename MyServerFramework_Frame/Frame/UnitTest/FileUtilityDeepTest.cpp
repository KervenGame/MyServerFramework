#include "FrameHeader.h"

namespace
{
	struct TempDirGuard
	{
		string root;
		explicit TempDirGuard(string dir) : root(move(dir)) {}
		~TempDirGuard()
		{
			deleteFolder(root);
		}
	};

	static void assertFileUtility(bool condition, const char* message)
	{
		if (!condition)
		{
			ERROR(string("FileUtilityDeepTest failed: ") + message);
		}
	}

	static string tempRoot()
	{
		return "FileUtilityDeepTest_tmp";
	}

	static void prepareTree(const string& root)
	{
		createFolder(root);
		createFolder(root + "/a");
		createFolder(root + "/b");
		createFolder(root + "/b/c");
		writeFile(root + "/root.txt", string("root"), false);
		writeFile(root + "/a/alpha.txt", string("alpha"), false);
		writeFile(root + "/a/beta.log", string("beta"), false);
		writeFile(root + "/b/c/gamma.txt", string("gamma"), false);
	}

	static void testPathAndExistence()
	{
		const string root = tempRoot();
		TempDirGuard guard(root);
		prepareTree(root);

		string path = "abc";
		validPath(path);
		assertFileUtility(path == "abc/", "validPath append");
		assertFileUtility(isDirExist(root), "root dir exists");
		assertFileUtility(isFileExist(root + "/root.txt"), "root file exists");
		assertFileUtility(!isFileExist(root + "/missing.txt"), "missing file not exists");
	}

	static void testFindAndCopy()
	{
		const string root = tempRoot();
		TempDirGuard guard(root);
		prepareTree(root);

		Vector<string> txtFiles;
		findFiles(root, txtFiles, string(".txt"), true);
		assertFileUtility(txtFiles.size() == 3, "txt file count");

		Vector<string> folders;
		findFolders(root, folders, true);
		assertFileUtility(folders.size() >= 3, "folder count");

		assertFileUtility(copyFile(root + "/root.txt", root + "/root_copy.txt", true), "copy file");
		assertFileUtility(isFileExist(root + "/root_copy.txt"), "copied file exists");

		assertFileUtility(renameFile(root + "/root_copy.txt", root + "/root_renamed.txt", true), "rename file");
		assertFileUtility(isFileExist(root + "/root_renamed.txt"), "renamed file exists");
		assertFileUtility(!isFileExist(root + "/root_copy.txt"), "old copy removed");

		assertFileUtility(copyFolder(root + "/a/", root + "/a_copy/", true), "copy folder");
		assertFileUtility(isDirExist(root + "/a_copy"), "copied folder exists");
		assertFileUtility(isFileExist(root + "/a_copy/alpha.txt"), "copied nested file");
	}

	static void testReadWriteAndMD5()
	{
		const string root = tempRoot();
		TempDirGuard guard(root);
		prepareTree(root);

		const string txt = root + "/sample.txt";
		const string content = "hello\nworld";
		assertFileUtility(writeFile(txt, content, false), "write text");
		assertFileUtility(getFileSize(txt) == (int)content.length(), "file size");

		string loaded = openTxtFile(txt, false, true);
		assertFileUtility(loaded == content, "open text");

		Vector<string> lines;
		assertFileUtility(openTxtFileLines(txt, lines, false, true), "open lines");
		assertFileUtility(lines.size() == 2, "line count");
		assertFileUtility(lines[0] == "hello", "line 0");
		assertFileUtility(lines[1] == "world", "line 1");

		const string md5A = generateMD5(content, false);
		const string md5B = generateFileMD5(txt, false);
		assertFileUtility(!md5A.empty(), "md5 memory");
		assertFileUtility(!md5B.empty(), "md5 file");
	}

	static void testDeleteHelpers()
	{
		const string root = tempRoot();
		TempDirGuard guard(root);
		prepareTree(root);

		const string emptyDir = root + "/empty";
		createFolder(emptyDir);
		assertFileUtility(deleteEmptyFolder(emptyDir), "delete empty folder");
		assertFileUtility(!isDirExist(emptyDir), "empty removed");

		deleteFile(root + "/root.txt");
		assertFileUtility(!isFileExist(root + "/root.txt"), "file deleted");
	}
}

void FileUtilityDeepTest::test()
{
	testPathAndExistence();
	testFindAndCopy();
	testReadWriteAndMD5();
	testDeleteHelpers();
}
