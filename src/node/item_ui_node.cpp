#include "item_ui_node.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/resource.hpp"

using namespace godot;

void InventoryItemUI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item", "item"), &InventoryItemUI::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &InventoryItemUI::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");
}

void InventoryItemUI::set_item(const Ref<InventoryItem> &p_item) {
	// If running in editor, create a deep duplicate to prevent recursion
	if (p_item.is_valid() && Engine::get_singleton()->is_editor_hint())
		item = p_item->duplicate_deep(Resource::DEEP_DUPLICATE_ALL);
	else
		item = p_item;
}

Ref<InventoryItem> InventoryItemUI::get_item() const {
	return item;
}
