package com.$${MOD_PACKAGE};

import com.$${MOD_PACKAGE}.init.*;
import com.$${MOD_PACKAGE}.proxy.*;
import com.$${MOD_PACKAGE}.util.*;

import net.minecraftforge.fml.common.*;
import net.minecraftforge.fml.common.Mod.*;
import net.minecraftforge.fml.common.event.*;

@Mod(useMetadata=true, modid = Reference.MOD_ID, name = Reference.NAME, version = Reference.VERSION)
public class $${MOD_ID} {
	@Instance
	public static $${MOD_ID} instance;

	@SidedProxy(clientSide = Reference.CLIENT_PROXY_CLASS, serverSide = Reference.COMMON_PROXY_CLASS)
	public static CommonProxy proxy;

    @EventHandler
	public void preInit(FMLPreInitializationEvent event) {

	}

	@EventHandler
	public void init(FMLInitializationEvent event) {

	}

	@EventHandler
	public void postInit(FMLPostInitializationEvent event) {

	}

	@EventHandler
	public void serverInit(FMLServerStartingEvent event)
	{

	}
}