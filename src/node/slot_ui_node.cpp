#include "slot_ui_node.hpp"
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "node/item_ui_node.hpp"
#include "resource/inventory_item.hpp"

using namespace godot;

void SlotUiNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slot", "slot"), &SlotUiNode::set_slot);
	ClassDB::bind_method(D_METHOD("get_slot"), &SlotUiNode::get_slot);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "slot", PROPERTY_HINT_RESOURCE_TYPE, "InventorySlot"), "set_slot", "get_slot");

	ClassDB::bind_method(D_METHOD("set_item_ui_holder", "holder"), &SlotUiNode::set_item_ui_holder);
	ClassDB::bind_method(D_METHOD("get_item_ui_holder"), &SlotUiNode::get_item_ui_holder);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_ui_holder", PROPERTY_HINT_NODE_TYPE), "set_item_ui_holder", "get_item_ui_holder");
}

void SlotUiNode::set_slot(Ref<InventorySlot> p_slot) {
	// If running in editor, create a deep duplicate to prevent recursion
	if (p_slot.is_valid() && Engine::get_singleton()->is_editor_hint())
		slot = p_slot->duplicate_deep(Resource::DEEP_DUPLICATE_ALL);
	else
		slot = p_slot;

	if (slot.is_valid())
		slot->connect("changed", callable_mp(this, &SlotUiNode::_update_item_ui));

	_update_item_ui();
}
Ref<InventorySlot> SlotUiNode::get_slot() const { return slot; }

void SlotUiNode::set_item_ui_holder(Node *p_holder) {
	item_ui_holder = p_holder;
	_update_item_ui();
}
Node *SlotUiNode::get_item_ui_holder() const { return item_ui_holder; }

void SlotUiNode::_update_item_ui() {
	if (item_ui_holder == nullptr)
		return;

	// Free children
	for (int32_t i = 0; i < item_ui_holder->get_child_count(); i++) {
		Node *child = item_ui_holder->get_child(i);
		item_ui_holder->remove_child(child);
		child->queue_free();
	}

	if (slot.is_null())
		return;

	const Ref<InventoryItem> item = slot->get_item();
	if (item.is_null())
		return;

	// Instantiate item ui scene
	ItemUiNode *item_ui = item->instantiate_item_ui();
	if (item_ui == nullptr)
		return;

	// Add item ui as child of item ui holder
	item_ui_holder->add_child(item_ui, false);
}

Variant SlotUiNode::_get_drag_data(const Vector2 &p_position) {
	if (slot.is_null() || slot->get_item().is_null())
		return Variant();

	set_drag_preview(_get_drag_preview());

	return slot;
}

Control *SlotUiNode::_get_drag_preview() {
    const Vector2 size = Vector2(50, 50);
    Control *holder = memnew(Control);
    holder->add_child(duplicate());
    holder->set_size(size);
    holder->set_anchors_and_offsets_preset(PRESET_CENTER);
    return holder;
}

bool SlotUiNode::_can_drop_data(const Vector2 &p_at_position, const Variant &p_data) const {
	Ref<InventorySlot> other_slot = p_data;
	return other_slot.is_valid() && other_slot != slot;
}

void SlotUiNode::_drop_data(const Vector2 &p_at_position, const Variant &p_data) {
	Ref<InventorySlot> other_slot = p_data;
	if (other_slot.is_null() || slot.is_null())
		return;

	other_slot->interact_item_from_slot(slot);
}
