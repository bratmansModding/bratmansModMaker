#include "lib//creation.cpp"

int main() {
	mod salut{
			.name = "Salut test",
			.version = "1.2",
			.author = "Bratman",
			.gameVersion = v1_12_2,
			.moddingApi = FORGE,
			.opt = {
							.description = "Example description",
							.url = R"(\"url\")",
							.credits = "myself",
							.logoFile = "../testures/uranium_ingot.png",
							.parent = "unused",
							.authors = {"greg"},
							.screenshots = {"hurr"},
							.dependencies = {"unused"}
			}
	};
	item uranium{
			.name = "Uranium Ingot",
			.texturePath = "../testures/uranium_ingot.png",
			.opt = {
					.desc = {"A radioactive ingot", "mmh yes quite radioactive"},
					.shiftDesc = {"Right click to explode"},
					.lore = {"This is a lore"}
			}
	};
	item silver{
			.name = "silver",
	.texturePath = "../testures/silver_ingot.png"
	};

	createMod(salut);
	createItem(uranium);
	createItem(silver);
	build();
}
