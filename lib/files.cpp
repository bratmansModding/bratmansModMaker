#include "files.h"

string crToCrLf(string text) {
    string::size_type pos = 0;
    while ((pos = text.find('\r', pos)) != string::npos)
    {
        text.replace(pos, 1, "\n");
        pos+=2;
    }
    return text;
}

void createFolderStructure(string path) {
    string truncatedPath;
    for (int i = (int)path.length(); i >= 0; --i) {
        if (path[i] == '/' || path[i] == '\\') {
            truncatedPath = path.substr(0, i);
            break;
        }
    }

    if (!exists(truncatedPath)) {
        class path dir(truncatedPath);
        if (!create_directories(dir)) {
            cerr << "Error: folder structure could not be created\n";
            return;
        }
    }
}

void writeFile(const string& data, const string& path, bool append = false) {
	createFolderStructure(path);
	std::ofstream outdata(path, append ? ios::app  : ios::out);

	if (!outdata.is_open()) {
		cerr << "Error: file could not be opened\n";
		return;
	}

    outdata << crToCrLf(data);
    outdata.close();

    cout << "File " << (append ? "appended" : "written") << " to " << path << '\n';
}

string readFile(const string& path) {
    std::ifstream indata(path);

    if (!indata.is_open()) {
        cerr << "Error: file could not be opened\n";
        return "";
    }

    string data;
    string line;
    while (getline(indata, line)) {
        data += line + '\n';
    }
    indata.close();

    return data;
}

void appendFileAfter(const string& data, const string& path, const string& after) {
    std::ifstream file(path, ios::out | ios::app | ios::binary);
    string text;

    if (!file.is_open()) {
        cerr << "Error: file could not be opened\n";
        return;
    }

    // Append data to the file after the given string
    string line;
    while (getline(file, line)) {
        text += line;
        if (line.find(after) != string::npos) {
            text += data;
        }
    }

    file.close();
    writeFile(text, path);
}

void copyForF(const string& src, const string& target) {
    createFolderStructure(target);

    try {
        if (is_directory(src)) {
            for (const auto& entry: recursive_directory_iterator(src)) {
                string path = entry.path().string();
                string newPath = target + path.substr(src.length());

                createFolderStructure(newPath);
                if (!is_directory(entry))
                    if (!exists(newPath))
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

void replaceInFile(const string& path, const map <string, string>& vars) {
    std::ifstream file(path, ios::out | ios::app | ios::binary);
    string text;
    string line;

    while (getline(file, line)) {
        bool found;
        text += replaceInString(line, vars, found);
    }

    file.close();
    writeFile(text, path);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
void replaceInForF(const string& path, const map <string, string>& vars) {
    for (const auto& entry : recursive_directory_iterator(path)) {
        bool isDirectory = entry.is_directory();
        bool isFile = entry.is_regular_file();
        if (isDirectory || isFile) {
            bool found;
            string newPath = replaceInString(entry.path().string(), vars, found);

            if (found) {
                rename(entry.path(), newPath);
                if (isDirectory) {
                    replaceInForF(newPath, vars);
                }
            }
            if (isFile)
                replaceInFile(newPath, vars);
        }
    }
    cout << "Files and folders renamed\n";
}
#pragma clang diagnostic pop

void cleanup() {
    remove_all("../tmp");
	remove_all(MOD_PATH);
    remove_all(GRADLE_PATH + "/src");
    remove_all(GRADLE_PATH + "/build");
	remove_all(GRADLE_PATH + "/build.gradle");
	cout << "Cleanup done\n";
}