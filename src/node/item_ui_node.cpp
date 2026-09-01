#include "item_ui_node.hpp"

using namespace godot;

void ItemUiNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item", "item"), &ItemUiNode::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &ItemUiNode::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");
}

void ItemUiNode::set_item(const Ref<InventoryItem> &p_item) {
	item = p_item;
}

Ref<InventoryItem> ItemUiNode::get_item() const {
	return item;
}
