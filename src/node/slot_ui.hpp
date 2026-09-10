#pragma once

#include "godot_cpp/classes/margin_container.hpp"
#include "resource/inventory_item.hpp"
#include "resource/inventory_slot.hpp"

namespace godot {

// SlotNode holds a slot
// handles instantiating the UI scene of the slot
class InventorySlotUI : public MarginContainer {
	GDCLASS(InventorySlotUI, MarginContainer)

private:
	Ref<InventorySlot> slot;
	Node *item_ui_holder;

protected:
	static void _bind_methods();

	void _update_item_ui();
	Control *_get_drag_preview();

public:
	void set_slot(Ref<InventorySlot> p_slot);
	Ref<InventorySlot> get_slot() const;

	void set_item(Ref<InventoryItem> p_item);
	Ref<InventoryItem> get_item() const;

	void set_item_ui_holder(Node *p_holder);
	Node *get_item_ui_holder() const;

	Variant _get_drag_data(const Vector2 &p_position) override;
	bool _can_drop_data(const Vector2 &p_position, const Variant &p_data) const override;
	void _drop_data(const Vector2 &p_position, const Variant &p_data) override;
};
} //namespace godot
