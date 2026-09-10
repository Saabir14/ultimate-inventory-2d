#pragma once

#include "godot_cpp/classes/packed_scene.hpp"

#include <godot_cpp/core/gdvirtual.gen.inc>

namespace godot {
class InventoryItemUI;

// Resource to store item data
// Extend to add properties
class InventoryItem : public Resource {
	GDCLASS(InventoryItem, Resource)

private:
    // Scene path of item UI when in inventory
	StringName item_ui_scene_path;

protected:
	static void _bind_methods();

public:
	void set_item_ui_scene_path(const StringName scene_path);
	StringName get_item_ui_scene_path() const;

	InventoryItemUI *instantiate_item_ui(PackedScene::GenEditState p_edit_state = PackedScene::GenEditState(0));

	// Adds another item to this item
	// This could be adding stack size
	// Or even placing it inside a nested inventory within current item
	// Override with a custom implementation that either:
	//  >> Combines given item with this item and returns true
	//  >> Doesn't do anything and returns false
	virtual bool place(const Ref<InventoryItem> item);
	GDVIRTUAL1RC(bool, place, Ref<InventoryItem>);

	// Implement custom item splitting behaviour
	virtual Ref<InventoryItem> take(int n = 1);

	// Implement custom item splitting behaviour
	virtual Ref<InventoryItem> take_half();

	// Implement custom item splitting behaviour
	virtual Ref<InventoryItem> take_one();

	// Implement custom item splitting behaviour
	virtual Ref<InventoryItem> take_all();
};

} //namespace godot
