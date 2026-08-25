#pragma once

#include "godot_cpp/classes/resource.hpp"
#include "resource/inventory_item.hpp"

namespace godot {

// Resource to store a single item data in a slot
// Used to process swapping of items bitween slots and other item UI interactions
class InventorySlot : public Resource {
	GDCLASS(InventorySlot, Resource)

private:
    // UI for InventorySlot
    // UI for InventoryItem will be child of this
    Ref<PackedScene> slot_ui_scene;

	// IvenentoryItem resource that stores item properties
	Ref<InventoryItem> item;

protected:
	static void _bind_methods();

public:
    void set_slot_ui_scene(const Ref<PackedScene> &p_scene);
    Ref<PackedScene> get_slot_ui_scene() const;

	void set_item(const Ref<InventoryItem> &p_item);
	Ref<InventoryItem> get_item() const;

	// Rakes an inventory item as input
	// Returns true if the item can be held in this slot otherwise false
	// Override to restrict what items can be held
	virtual bool _can_hold_item(const Ref<InventoryItem> &p_item);
	GDVIRTUAL1RC(bool, _can_hold_item, Ref<InventoryItem>);

	// Takes an inventory slot as input
	// Swaps the item in both slots
	// Returns true if swap was succesfull
	// Returns false if no swap
	bool swap_item_from_slot(const Ref<InventorySlot> &p_slot);

	// Takes an inventory slot as input
	// Place item in slot
	// Remove item from input slot if succesfull
	// Returns true if item placed otherwise false
	bool place_item_from_slot(const Ref<InventorySlot> &p_slot);

	// Takes an inventory slot as input
	// Tries to place the item in inventory with item's [TODO: use inventory_item's actual function name] method
	// If this fails, try swapping the items instead
	// If this fails return false
	// This is how you would tipically interact with inventory
	// when a user tries to move one slot onto another
	bool interact_item_from_slot(const Ref<InventorySlot> &p_slot);
};

} //namespace godot
