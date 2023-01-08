#include "files.h"

/**
 * Replaces all cr with crlf
 * @param text Text to replace
 * @return Text with crlf instead of cr
 */
string crToCrLf(string text) {
	string::size_type pos = 0;
	while ((pos = text.find('\r', pos)) != string::npos) {
		text.replace(pos, 1, "\n");
		pos += 2;
	}
	return text;
}

/**
 * Truncates a path to to everything but the file name without the trailing slash
 * @param path Path to truncate
 * @return Truncated path
 */
string truncatePath(string path) {
    string::size_type pos = path.find_last_of("\\/");
    return (pos == string::npos) ? path : path.substr(0, pos);
}

/**
 * Creates a folder structure of all missing folders in the given path.
 * @param path The path to create.
 */
void createFolderStructure(string path) {
	string truncatedPath = truncatePath(path);

	if (!exists(truncatedPath)) {
		class path dir(truncatedPath);
		if (!create_directories(dir)) {
			cerr << "Error: folder structure could not be created\n";
			return;
		}
	}
}

/**
 * Opens a file
 * @param path The path to the file
 * @param mode The mode to open the file in
 * @return The file stream
 */
fstream openFile(string path, std::ios_base::openmode mode) {
    fstream file(path, mode);

    if (!file.is_open()) {
        cerr << "Error: file could not be opened\n";
        throw "File could not be opened";
    }

    return file;
}

/**
 * Reads a file and returns its contents as a string
 * @param path The path to the file
 * @return The contents of the file
 */
string readFile(const string& path) {
    fstream file = openFile(path, ios::in);
    string text;

    return text.assign((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());;
}

/**
 * Writes text to a file
 * @param data The text to write
 * @param path The path to the file
 * @param append Whether to append to the file or overwrite it
 */
void writeFile(const string& data, const string& path, bool append = false) {
    createFolderStructure(path);

    openFile(path, append ? ios::app : ios::out) << crToCrLf(data);

    cout << "File written to " << path << '\n';
}

/**
 * Appends data to a file after a given string is found
 * @param data The data to append
 * @param path The path to the file
 * @param after The string to find and append after
 */
void appendFileAfter(const string& data, const string& path, const string& after) {
    if (after.empty())
        return writeFile(data, path, true);

    string text = readFile(path);
    string::size_type pos;
    if ((pos = text.find(after)) == string::npos) {
        cerr << "Error: could not find \"" << after << "\" in " << path << '\n';
        return;
    }

    text.insert(pos + after.length(), '\n' + data);
    writeFile(text, path);
}

/**
 * Copies a file or a folder to a new location
 * @param src The file/folder to copy
 * @param target The new location
 */
void copyFileOrFolder(const string& src, const string& target) {
	createFolderStructure(target);

	try {
		if (is_directory(src)) {
			for (const auto& entry : recursive_directory_iterator(src)) {
				string path = entry.path().string();
				string newPath = target + path.substr(src.length());

				createFolderStructure(newPath);

                // Ignore files that are already in the target folder
				if (is_regular_file(entry) && !exists(newPath))
					copy_file(path, newPath);
			}
			cout << "Folder";
		} else {
			copy_file(src, target);
			cout << "File";
		}

		cout << " copied from " << src << " to " << target << '\n';
	} catch (const std::exception& e) {
		cerr << e.what() << '\n';
	}
}

/**
 * Replaces all occurrences of a value within a string with another value from the map
 * @param s The string to replace in
 * @param vars The map of values to replace($${first}) with the replacement(second)
 * @param found A pointer to a bool that will be set to true if a replacement was made
 * @return The string with all replacements made
 */
string replaceInString(string s, const map <string, string>& vars, bool& found) {
	found = false;

	for (const auto& t : vars) {
		string tofind = "$${" + t.first + "}";
		size_t pos = s.find(tofind);

		while (pos != string::npos) {
			s.replace(pos, tofind.size(), t.second);
			pos = s.find(tofind, pos + t.second.size());
			found = true;
		}
	}

	return s;
}

/**
 * Replaces all occurrences of a value within a file with another value from the map
 * @param path The path to the file to replace in
 * @param vars The map of values to replace($${first}) with the replacement(second)
 */
void replaceInFile(const string& path, const map <string, string>& vars) {
    bool found;
	writeFile(replaceInString(readFile(path), vars, found), path);
}

/**
 * Replaces all occurrences of a value within a folder(file/folder names and file data) with another value from the map
 * @param path The path to the folder to replace in
 * @param vars The map of values to replace($${first}) with the replacement(second)
 */
void replaceInFileOrFolder(const string& path, const map <string, string>& vars) {
	for (const auto& entry : recursive_directory_iterator(path)) {
		bool isDirectory = entry.is_directory(), isFile = entry.is_regular_file();

		if (isDirectory || isFile) {
			bool found;
			string newPath = replaceInString(entry.path().string(), vars, found);

			if (found) {
				rename(entry.path(), newPath);
				if (isDirectory) {
                    replaceInFileOrFolder(newPath, vars);
				}
			}
			if (isFile)
				replaceInFile(newPath, vars);
		}
	}
}

/**
 * Deletes all temporary files
 */
void cleanup() {
    string paths[] = {"../tmp",
                      MOD_PATH,
                      GRADLE_PATH + "/src",
                      GRADLE_PATH + "/build",
                      GRADLE_PATH + "/build.gradle"};

    for (const string& path : paths)
        remove_all(path);

	cout << "Cleanup done\n";
}
