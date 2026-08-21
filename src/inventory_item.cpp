#include "inventory_item.hpp"

#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

void InventoryItem::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item_ui_scene", "item_ui_scene"), &InventoryItem::set_item_ui_scene);
	ClassDB::bind_method(D_METHOD("get_item_ui_scene"), &InventoryItem::get_item_ui_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_ui_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_item_ui_scene", "get_item_ui_scene");

	ClassDB::bind_method(D_METHOD("set_item_2d_scene", "item_2d_scene"), &InventoryItem::set_item_2d_scene);
	ClassDB::bind_method(D_METHOD("get_item_2d_scene"), &InventoryItem::get_item_2d_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_2d_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_item_2d_scene", "get_item_2d_scene");
}

InventoryItem::InventoryItem() { set_local_to_scene(true); }

void InventoryItem::set_item_ui_scene(const Ref<PackedScene> &p_scene) { item_ui_scene = p_scene; }
Ref<PackedScene> InventoryItem::get_item_ui_scene() const { return item_ui_scene; }

void InventoryItem::set_item_2d_scene(const Ref<PackedScene> &p_scene) { item_2d_scene = p_scene; }
Ref<PackedScene> InventoryItem::get_item_2d_scene() const { return item_2d_scene; }
