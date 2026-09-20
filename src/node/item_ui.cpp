#include "item_ui.hpp"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/core/error_macros.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

void ItemUI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item", "item"), &ItemUI::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &ItemUI::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");
}

void ItemUI::set_item(const Ref<InventoryItem> &p_item) { item = p_item; }
Ref<InventoryItem> ItemUI::get_item() const { return item; }
