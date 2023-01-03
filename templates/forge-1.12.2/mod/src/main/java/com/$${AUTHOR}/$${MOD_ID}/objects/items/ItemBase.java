package com.$${MOD_PACKAGE}.objects.items;

import java.util.List;

import com.$${MOD_PACKAGE}.$${MOD_ID};
import com.$${MOD_PACKAGE}.init.ItemInit;
import com.$${MOD_PACKAGE}.util.IHasModel;

import net.minecraft.creativetab.CreativeTabs;
import net.minecraft.client.util.ITooltipFlag;
import net.minecraft.world.World;
import net.minecraft.item.*;

public class ItemBase extends Item implements IHasModel {
    public ItemBase(String name) {
		setUnlocalizedName(name);
		setRegistryName(name);
		setCreativeTab(CreativeTabs.MISC);

		ItemInit.ITEMS.add(this);
	}

    @Override
    public void registerModels() {
        $${MOD_ID}.proxy.registerItemRenderer(this, 0, "inventory");
	}
}