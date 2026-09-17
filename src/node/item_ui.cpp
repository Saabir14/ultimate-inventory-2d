#include "item_ui.hpp"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

void InventoryItemUI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item", "item"), &InventoryItemUI::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &InventoryItemUI::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");
}

void InventoryItemUI::set_item(const Ref<InventoryItem> &p_item) {
	if (item.is_valid())
		item->disconnect("item_ui_scene_path_set", callable_mp(this, &InventoryItemUI::_on_item_ui_scene_path_set));

	item = p_item;

	if (item.is_valid())
		item->connect("item_ui_scene_path_set", callable_mp(this, &InventoryItemUI::_on_item_ui_scene_path_set));
}
Ref<InventoryItem> InventoryItemUI::get_item() const { return item; }

void InventoryItemUI::_on_item_ui_scene_path_set(const StringName &p_scene_path) {
    for (int32_t i = 0; i < get_child_count(); i++)
        get_child(i)->queue_free();

    replace_by(item->instantiate_item_ui(PackedScene::GEN_EDIT_STATE_INSTANCE));
}
