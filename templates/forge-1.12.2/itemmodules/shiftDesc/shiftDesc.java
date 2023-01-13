    @Override
	public void addInformation(ItemStack stack, World worldIn, List<String> desc, ITooltipFlag flagIn) {
		if (GuiScreen.isShiftKeyDown()) {
$${ITEM_SHIFT_DESC}
		} else {
$${ITEM_DESC}
		}
	}