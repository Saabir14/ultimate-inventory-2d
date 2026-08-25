#pragma once

#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/resource.hpp"

#include <godot_cpp/core/gdvirtual.gen.inc>

namespace godot {

// Resource to store item data
// Extend to add properties
class InventoryItem : public Resource {
	GDCLASS(InventoryItem, Resource)

private:
    // Scene of item UI when in inventory
	Ref<PackedScene> item_ui_scene;
	// 2d scene of item when dropped
	Ref<PackedScene> item_2d_scene;

protected:
	static void _bind_methods();

public:
	void set_item_ui_scene(const Ref<PackedScene> &p_scene);
	Ref<PackedScene> get_item_ui_scene() const;

	void set_item_2d_scene(const Ref<PackedScene> &p_scene);
	Ref<PackedScene> get_item_2d_scene() const;

	// Function to check if the item should be freed
	// Used to check if item should be deleted
	// By default allways returns false
	// Override for custom valid check behaviour
	// Example use: override free_ready() to return false when:
	//  >> Item durability of consumable item returns 0
	//  >> Stack size of item is 0
	virtual bool free_ready();
	GDVIRTUAL0RC(bool, free_ready);

	// Adds another item to this item
	// This could be adding stack size
	// Or even placing it inside a nested inventory within current item
	// Override with a sutom implementation that either:
	//  >> Combines given item with this item and returns true
	//  >> Doesn't do anything and returns false
	// No need to free item after combining
	// If you implemented _is_valid(),
	// inventory_slot will free automatically if needed
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
