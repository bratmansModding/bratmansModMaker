    @Override
	public void addInformation(ItemStack stack, World worldIn, List<String> desc, ITooltipFlag flagIn) {
		if (GuiScreen.isShiftKeyDown()) {
			desc.add("$${ITEM_SHIFT_DESC}");
		} else {
			desc.add("$${ITEM_DESC}");
		}
	}