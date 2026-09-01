#include "inventory_item.hpp"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "node/item_ui_node.hpp"

using namespace godot;

void InventoryItem::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item_ui_scene", "item_ui_scene"), &InventoryItem::set_item_ui_scene);
	ClassDB::bind_method(D_METHOD("get_item_ui_scene"), &InventoryItem::get_item_ui_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_ui_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_item_ui_scene", "get_item_ui_scene");

	ClassDB::bind_method(D_METHOD("set_item_2d_scene", "item_2d_scene"), &InventoryItem::set_item_2d_scene);
	ClassDB::bind_method(D_METHOD("get_item_2d_scene"), &InventoryItem::get_item_2d_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_2d_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_item_2d_scene", "get_item_2d_scene");

	ClassDB::bind_method(D_METHOD("instantiate_item_ui"), &InventoryItem::instantiate_item_ui, DEFVAL(0));

	GDVIRTUAL_BIND(place, "item");

	// TODO: virtual bind all take functions
}

void InventoryItem::set_item_ui_scene(const Ref<PackedScene> &p_scene) {
    // Check if p_scene contains ItemUiNode as root node
	if (p_scene.is_valid()) {
		Node *node = p_scene->instantiate();
		ItemUiNode *item_ui_node = Object::cast_to<ItemUiNode>(node);
		ERR_FAIL_COND_MSG(!item_ui_node, "set_item_ui_scene scene must have ItemUiNode as root node");
	}

	item_ui_scene = p_scene;
}
Ref<PackedScene> InventoryItem::get_item_ui_scene() const { return item_ui_scene; }

void InventoryItem::set_item_2d_scene(const Ref<PackedScene> &p_scene) { item_2d_scene = p_scene; }
Ref<PackedScene> InventoryItem::get_item_2d_scene() const { return item_2d_scene; }

ItemUiNode *InventoryItem::instantiate_item_ui(PackedScene::GenEditState p_edit_state) {
    if (item_ui_scene.is_null())
		return nullptr;

	Node *node = item_ui_scene->instantiate(p_edit_state);

	// node has to be ItemUiNode
	ItemUiNode *item_ui_node = Object::cast_to<ItemUiNode>(node);
	if (item_ui_node == nullptr)
		return nullptr;

	item_ui_node->set_item(this);
	return item_ui_node;
}

bool InventoryItem::place(const Ref<InventoryItem> item) { return false; }

Ref<InventoryItem> InventoryItem::take(int n) { return nullptr; }
Ref<InventoryItem> InventoryItem::take_half() { return nullptr; }
Ref<InventoryItem> InventoryItem::take_one() { return nullptr; }
Ref<InventoryItem> InventoryItem::take_all() { return nullptr; }
