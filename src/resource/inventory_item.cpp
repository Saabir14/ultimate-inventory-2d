#include "inventory_item.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "node/item_ui.hpp"

#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/placeholder_texture2d.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/resource_uid.hpp"
#include "godot_cpp/classes/texture_rect.hpp"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/string_name.hpp"

using namespace godot;

void InventoryItem::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item_ui_scene_path", "item_ui_scene_path"), &InventoryItem::set_item_ui_scene_path);
	ClassDB::bind_method(D_METHOD("get_item_ui_scene_path"), &InventoryItem::get_item_ui_scene_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_ui_scene_path", PROPERTY_HINT_FILE, "*.tscn, *.scn, *.res"), "set_item_ui_scene_path", "get_item_ui_scene_path");

	ClassDB::bind_method(D_METHOD("instantiate_item_ui"), &InventoryItem::instantiate_item_ui, DEFVAL(0));

	GDVIRTUAL_BIND(place, "item");
	GDVIRTUAL_BIND(take, "n");
	GDVIRTUAL_BIND(take_half);
	GDVIRTUAL_BIND(take_one);
	GDVIRTUAL_BIND(take_all);
}

void InventoryItem::set_item_ui_scene_path(const StringName scene_path) {
	if (item_ui_scene_path == scene_path)
		return;

	item_ui_scene_path = scene_path;
}
StringName InventoryItem::get_item_ui_scene_path() const { return item_ui_scene_path; }

ItemUI *InventoryItem::instantiate_item_ui(PackedScene::GenEditState p_edit_state) {
	if (item_ui_scene_path.is_empty())
		return instantiate_default_item_ui(p_edit_state);

	const StringName path = ResourceUID::ensure_path(item_ui_scene_path);
	ERR_FAIL_COND_V_MSG(!ResourceLoader::get_singleton()->exists(path), instantiate_default_item_ui(p_edit_state), "No file found at " + path);

	const Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(path);
	ERR_FAIL_COND_V_MSG(scene.is_null(), instantiate_default_item_ui(p_edit_state), "File is not PackedScene at " + path);

	Node *node = scene->instantiate(p_edit_state);

	// node has to be ItemUiNode
	ItemUI *item_ui_node = Object::cast_to<ItemUI>(node);
	ERR_FAIL_NULL_V_MSG(item_ui_node, instantiate_default_item_ui(p_edit_state), "No ItemUI as root node for PackedScene at " + path);

	item_ui_node->set_item(this);
	return item_ui_node;
}
ItemUI *InventoryItem::instantiate_default_item_ui(PackedScene::GenEditState p_edit_state) {
	ItemUI *item_ui = memnew(ItemUI);
	TextureRect *texture2d = memnew(TextureRect);
	texture2d->set_texture(memnew(PlaceholderTexture2D));
	item_ui->add_child(texture2d);
	item_ui->set_item(this);
	return item_ui;
}

bool InventoryItem::place(const Ref<InventoryItem> item) { return false; }

Ref<InventoryItem> InventoryItem::take(int n) {
	Ref<InventoryItem> result = nullptr;
	GDVIRTUAL_CALL(take, n, result);
	return result;
}
Ref<InventoryItem> InventoryItem::take_half() {
	Ref<InventoryItem> result = nullptr;
	GDVIRTUAL_CALL(take_half, result);
	return result;
}
Ref<InventoryItem> InventoryItem::take_one() {
	Ref<InventoryItem> result = nullptr;
	GDVIRTUAL_CALL(take_one, result);
	return result;
}
Ref<InventoryItem> InventoryItem::take_all() {
	Ref<InventoryItem> result = nullptr;
	GDVIRTUAL_CALL(take_all, result);
	return result;
}
