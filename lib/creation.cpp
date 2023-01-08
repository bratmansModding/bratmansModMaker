#include <ranges>
#include "creation.h"
#include "files.cpp"

string toLowerNoNonAlnum(const string& text) {
	string result;
	for (char c : text) {
		if (isalnum(c)) {
			result += tolower(c);
		}
	}
	return result;
}

string toSnakeCase(const string& text, bool screaming = false) {
	string result;
	for (char c : text) {
		result += (c == ' ') ? '_' : screaming ? toupper(c) : tolower(c);
	}
	return result;
}

string toPascalCase(const string& text) {
	string result;
	bool nextUpper = true;
	for (char c : text) {
		if (c == ' ') {
			nextUpper = true;
		} else if (nextUpper) {
			result += toupper(c);
			nextUpper = false;
		} else {
			result += tolower(c);
		}
	}
	return result;
}

string formatArray(string s[], const string& prefix = "\"", const string& suffix = "\"", const string& separator = ", ") {
	string text;
	int arraySize = s->size();

	for (int i = 0; i < arraySize; i++) {
		if (s[i].empty())
			break;

        text.append(prefix).append(s[i]).append(suffix);
		if (i < arraySize - 1 && !s[i + 1].empty())
            text += separator;
	}

	return text;
}

void addModule(const string& modulePath, const string& filePath, map <string, string> &vars, const string& after = "") {
    bool found;

    appendFileAfter(
            replaceInString(
                    readFile(TEMPLATE_PATH + modulePath)
            , vars, found)
    , filePath, after);

    // Automatically add imports
    for (const auto& entry : directory_iterator(TEMPLATE_PATH + truncatePath(modulePath))) {
        string path = entry.path().string();
        if (path.find("imports.java") != string::npos)
            appendFileAfter(after + '\n' + readFile(path), filePath, "// additional imports");
    }
}

void createMod(mod mod) {
	MOD_ID = mod.opt.id.empty() ? toLowerNoNonAlnum(mod.name) : mod.opt.id;
	MOD_PATH = "../output/" + mod.name + "-bMM";
	MOD_VERSION = mod.version;
	TEMPLATE_PATH = "../templates/" + moddingApiMap[MODDING_API] + '-' + gameVersionMap[mod.gameVersion];
	AUTHOR = toLowerNoNonAlnum(mod.author);
	GAME_VERSION = mod.gameVersion;
	MODDING_API = mod.moddingApi;

	map <string, string> modMap = {
					{"MOD_VERSION", MOD_VERSION},
					{"MOD_NAME", mod.name},
					{"MOD_ID", MOD_ID},
					{"AUTHOR", AUTHOR},
					{"GAME_VERSION", gameVersionMap[mod.gameVersion]},
					{"MOD_PACKAGE", AUTHOR + '.' + MOD_ID},
					// opts
					{"MOD_DESCRIPTION", mod.opt.description},
					{"MOD_URL", mod.opt.url},
					{"MOD_UPDATE_URL", mod.opt.updateUrl},
					{"MOD_CREDITS", mod.opt.credits},
					{"MOD_LOGO_FILE", mod.opt.logoFile},
					{"MOD_PARENT", mod.opt.parent},
					{"AUTHORS", formatArray(mod.opt.authors)},
					{"MOD_SCREENSHOTS", formatArray(mod.opt.screenshots)},
					{"MOD_DEPENDENCIES", formatArray(mod.opt.dependencies)}
	};

	// Cleanup old files before writing new ones
	cleanup();

	// Copy everything from the template to ouput and replace all the variables in it
    copyFileOrFolder(TEMPLATE_PATH + "/mod", MOD_PATH);
    replaceInFileOrFolder(MOD_PATH, modMap);
}

void createItem(item item) {
	string ITEM_ID = item.opt.id.empty() ? toSnakeCase(item.name) : item.opt.id;
	string ITEM_CLASS = item.isBasic() ? "ItemBase" : toPascalCase(item.name);
	string ITEM_FINAL = toSnakeCase(item.name, true);
	map <string, string> itemMap = {
					{"MOD_PACKAGE", AUTHOR + '.' + MOD_ID},
					{"MOD_ID", MOD_ID},
					{"AUTHOR", AUTHOR},
					{"ITEM_ID", ITEM_ID},
					{"ITEM_NAME", item.name},
					{"ITEM_CLASS", ITEM_CLASS},
                    {"ITEM_FINAL", ITEM_FINAL}
	};

	// Paths
	string tmpPath = "../tmp/" + ITEM_ID;
	string itemPath = MOD_PATH + R"(\src\main\resources\assets\)" + MOD_ID;
	string mainPath = MOD_PATH + R"(\src\main\java\com\)" + AUTHOR + '\\' + MOD_ID;

	// Copy everything from the template to tmp, replace everything in it, then copy it to the mod
    copyFileOrFolder(TEMPLATE_PATH + "/item", tmpPath);
    replaceInFileOrFolder(tmpPath, itemMap);
    copyFileOrFolder(tmpPath, MOD_PATH);

	// Add the item in iteminit
    addModule("/itemmodules/init.java", mainPath + "/init/ItemInit.java", itemMap, "// init");

	// Add the item to the lang file
    addModule("/itemmodules/lang.lang",
              MOD_PATH + "/src/main/resources/assets/" + MOD_ID + "/lang/en_us.lang", itemMap);

	// Copy texture
    copyFileOrFolder(item.texturePath, itemPath + R"(\textures\items\)" + ITEM_ID + ".png");

	if (ITEM_CLASS != "ItemBase") {
        bool shiftDesc = !item.opt.shiftDesc[0].empty(), desc = !item.opt.desc[0].empty();

        if (shiftDesc || desc) {
            string modulePath = shiftDesc ? "/itemmodules/shiftDesc/shiftDesc.java" : "/itemmodules/desc.java";
            map <string, string> descMap = {
                    {"ITEM_DESC", formatArray(item.opt.desc, "", "", "\\n")},
                    {"ITEM_SHIFT_DESC", formatArray(item.opt.shiftDesc, "", "", "\\n")}
            };

            addModule(modulePath,
                      mainPath + "/objects/items/" + ITEM_CLASS + ".java", descMap, "// desc");
        }
	}
}

void build(const string& s = "build") {
	// Supprimer les anciens fichiers dans gradleready et passer les nouveaux
    copyFileOrFolder(MOD_PATH + "/src", GRADLE_PATH + "/src/");
    copyFileOrFolder(MOD_PATH + "/build.gradle", GRADLE_PATH + "/build.gradle");

	// Build
	system("echo ----------------");
	system(("cd " + GRADLE_PATH + " && .\\gradlew " + s).c_str());
	system("echo ----------------");

	// Puis déplacer le mod qui vient d'être build dans l'output
	if (s == "build") {
		try {
			string modJarName = MOD_ID + '-' + MOD_VERSION + ".jar";
			string outputModPath = "../output/" + modJarName;

			remove(outputModPath);
			copy_file(GRADLE_PATH + "/build/libs/" + modJarName,
				outputModPath);

			// ree
			string minecraftModPath = "C:/Users/micha/AppData/Roaming/.minecraft/mods/" + modJarName;
			remove(minecraftModPath);
			copy_file(outputModPath, minecraftModPath);
		} catch (const std::exception& e) {
			cout << "Build Failed";
		}
	}
}
