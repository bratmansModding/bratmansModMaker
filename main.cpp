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
    item darkSteelSword{
        .name = "The Ender",
		.texturePath = "../testures/dark_steel_sword.png",
        .opt = {
            .id = "item_dark_steel_sword",
            .desc = {"\\u00A7f\\u00A7o<Hold Shift>"},
            .shiftDesc = {"Increased skull and ender pearl drops",
                          "When powered: Endermen can't teleport once hit, extra Damage and Speed",
                          "\\u00A7ePossible Upgrades:"},
            .attributes = {
                .damage = 7,
                .attackSpeed = 1.6,
                .durability = 2000,
                .resource = {
                    .enabled = true,
                    .capacity = 0,
                    .type = attributes::resource::ENERGY
                }
            }
        }
    };
    item greg{
        .name = "Greg",
        .texturePath = "../testures/silver_ingot.png",
        .opt = {
            .desc = {"\\u00A7f\\u00A7ogreg"}
        }
    };
    
    createMod(salut);
    createItem(uranium);
    createItem(darkSteelSword);
    createItem(greg);
    build();
}
