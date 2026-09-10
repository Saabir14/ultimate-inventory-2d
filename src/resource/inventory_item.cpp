#include "inventory_item.hpp"

#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/resource_uid.hpp"
#include "godot_cpp/variant/string_name.hpp"
#include "node/item_ui.hpp"

using namespace godot;

void InventoryItem::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item_ui_scene_path", "item_ui_scene_path"), &InventoryItem::set_item_ui_scene_path);
	ClassDB::bind_method(D_METHOD("get_item_ui_scene_path"), &InventoryItem::get_item_ui_scene_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_ui_scene_path", PROPERTY_HINT_FILE, "*.tscn, *.scn, *.res"), "set_item_ui_scene_path", "get_item_ui_scene_path");

	ClassDB::bind_method(D_METHOD("instantiate_item_ui"), &InventoryItem::instantiate_item_ui, DEFVAL(0));

	GDVIRTUAL_BIND(place, "item");

	// TODO: virtual bind all take methods
}

void InventoryItem::set_item_ui_scene_path(const StringName scene_path) { item_ui_scene_path = scene_path; }
StringName InventoryItem::get_item_ui_scene_path() const { return item_ui_scene_path; }

InventoryItemUI *InventoryItem::instantiate_item_ui(PackedScene::GenEditState p_edit_state) {
	const StringName path = ResourceUID::ensure_path(item_ui_scene_path);
	ERR_FAIL_COND_V_MSG(!ResourceLoader::get_singleton()->exists(path), nullptr, "No file found at " + path);

	const Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(path);
	ERR_FAIL_COND_V_MSG(!scene.is_valid(), nullptr, "File is not PackedScene at " + path);

	Node *node = scene->instantiate(p_edit_state);

	// node has to be ItemUiNode
	InventoryItemUI *item_ui_node = Object::cast_to<InventoryItemUI>(node);
	ERR_FAIL_COND_V_MSG(!item_ui_node, nullptr, "No ItemUI as root node for PackedScene at " + path);

	item_ui_node->set_item(this);
	return item_ui_node;
}

bool InventoryItem::place(const Ref<InventoryItem> item) { return false; }

Ref<InventoryItem> InventoryItem::take(int n) { return nullptr; }
Ref<InventoryItem> InventoryItem::take_half() { return nullptr; }
Ref<InventoryItem> InventoryItem::take_one() { return nullptr; }
Ref<InventoryItem> InventoryItem::take_all() { return nullptr; }
