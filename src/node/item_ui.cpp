#include "item_ui.hpp"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

void ItemUI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item", "item"), &ItemUI::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &ItemUI::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");
}

void ItemUI::set_item(const Ref<InventoryItem> &p_item) {
	if (item.is_valid())
		item->disconnect("item_ui_scene_path_set", callable_mp(this, &ItemUI::_on_item_ui_scene_path_set));

	item = p_item;

	if (item.is_valid())
		item->connect("item_ui_scene_path_set", callable_mp(this, &ItemUI::_on_item_ui_scene_path_set));
}
Ref<InventoryItem> ItemUI::get_item() const { return item; }

void ItemUI::_on_item_ui_scene_path_set(const StringName &p_scene_path) {
	Node *parent = get_parent();
	if (!parent)
		return;

	int32_t index = get_index();
	queue_free();

	Node *new_node = item->instantiate_item_ui(PackedScene::GEN_EDIT_STATE_INSTANCE);
	parent->call_deferred("add_child", new_node);
	parent->call_deferred("move_child", new_node, index);
}
