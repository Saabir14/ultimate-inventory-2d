#pragma once

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "resource/inventory_item.hpp"
#include "resource/inventory_slot.hpp"

namespace godot {

// Inventory resource to store inventory items in a list of slots
// Use this for any inentory containing more than one item
class Inventory : public Resource {
	GDCLASS(Inventory, Resource)

private:
    // Template slot to use for all array slots
    Ref<InventorySlot> slot;

	TypedArray<Ref<InventorySlot>> slots;
	void _update_slots();

protected:
	static void _bind_methods();

public:
    void set_slot(const Ref<InventorySlot> &p_slot);
    Ref<InventorySlot> get_slot() const;

    void set_items(const TypedArray<Ref<InventoryItem>> &p_items);
    TypedArray<Ref<InventoryItem>> get_items() const;

    // Sets the size of the inventory
    // If the size is smaller, deletes ending items
    // Use attempt_resize and resize to
    // resize inventory while handeling overflowing items
    // Would not recomend using this
    void set_size(int64_t size);
    int64_t get_size() const;

    // Attepmts to resize the inventory to the given size
    // If the size is smaller, moves items into previous empty spaces
    // If still to small, reverts changes and returns false
    // returns true on success
    // Usefull if you wanna  resize but only do it if enough space
    bool attempt_resize(int64_t size);

    // Resizes inventory to the given size
    // If size is smaller than inventory size,
    // Packs inventory tightly and returns
    // The ending reminders in an array of inventory items
    // Usefull if some inventory change forces resize
    // and drops all excess items
    TypedArray<Ref<InventoryItem>> resize(int64_t size);

    bool place_item(const Ref<InventoryItem> &p_item, int64_t index = -1);
    bool place_item_from_slot(const Ref<InventorySlot> &p_slot, int64_t index = -1);
    bool swap_item_from_slot(const Ref<InventorySlot> &p_slot, int64_t index);
};

} //namespace godot
