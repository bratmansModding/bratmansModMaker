#include "files.h"

// funni
#define WITH(x) { \
        auto &_ = x;
#define END_WITH() }

// Enums
enum eGameVersion {
	v1_7_10,
	v1_12_2,
	v1_18_2
};
enum eModdingApi {
	FORGE,
	FABRIC
};

// Paths
string MOD_PATH;
string TEMPLATE_PATH;
string GRADLE_PATH = "../gradle";

// Mod info
string MOD_VERSION;
string MOD_ID;
string AUTHOR;
eGameVersion GAME_VERSION;
map <eGameVersion, string> gameVersionMap = {
		{v1_7_10, "1.7.10"},
		{v1_12_2, "1.12.2"},
		{v1_18_2, "1.18.2"}
};
eModdingApi MODDING_API;
map <eModdingApi, string> moddingApiMap = {
		{FORGE, "forge"},
		{FABRIC, "fabric"}
};

// Structs
struct mod {
	string name;
	string version;
	string author;
	eGameVersion gameVersion;
	eModdingApi moddingApi;
	struct opt {
		string id;
		string description;
		string url;
		string updateUrl;
		string credits;
		string logoFile;
		string parent;
		string authors[16];
		string screenshots[16];
		string dependencies[16];
	};
	opt opt;
};
struct attributes {
	int damage = 0;
	int projectileDamage = 0;
	int reach = 0;
	int miningSpeed = 0;
	float attackSpeed = 0;
	int durability = 0;
	int protection = 0;
	int thoughness = 0;
	int health = 0;
	struct resource {
		bool enabled = false;
		int capacity = 100;
		enum eResourceType {
			ENERGY,
			MANA
		};
		eResourceType type;
	};
	resource resource;
};
struct item {
	string name;
	string texturePath;
	struct opt {
        string id;
		string desc[16];
		string shiftDesc[16];
		string lore[16];
		string creativeTab;
		attributes attributes;
	};
	opt opt;
	enum eItemType {
		NONE,
		SWORD,
		BOW,
		PICKAXE,
		AXE,
		SHOVEL,
		HOE,
		COMSUMABLE,
		THROWABLE,
		NEW_TYPE,
		HELMET,
		CHESTPLATE,
		LEGGINGS,
		BOOTS
	};
	eItemType type = NONE;
	bool isBasic() {
		bool opts, attrs;

		WITH(this->opt)
			opts = _.desc[0].empty() && _.shiftDesc[0].empty() && _.lore[0].empty() && _.creativeTab.empty();
		END_WITH()

			WITH(this->opt.attributes)
			attrs = _.damage == 0 && _.projectileDamage == 0 && _.reach == 0 && _.miningSpeed == 0 && _.attackSpeed == 0
			&& _.durability == 0 && _.protection == 0 && _.thoughness == 0 && _.health == 0 && !_.resource.enabled;
		END_WITH()

			return opts && attrs && this->type == NONE;
	}
};
struct block {
	item item;
	// someday
};
