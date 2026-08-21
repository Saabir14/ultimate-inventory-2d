#pragma once

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "inventory_holder.hpp"

namespace godot {

class InventoryUI : public Node {
	GDCLASS(InventoryUI, Node)

private:
	InventoryHolder *inventory_holder = nullptr;
	Ref<PackedScene> item_slot_scene;

protected:
	static void _bind_methods();

public:
	void set_inventory_holder(InventoryHolder *p_inventory_holder);
	InventoryHolder *get_inventory_holder() const;

	void set_item_slot_scene(const Ref<PackedScene> &p_scene);
	Ref<PackedScene> get_item_slot_scene() const;

	void set_inventory(const Ref<Inventory> &p_inventory);
	Ref<Inventory> get_inventory() const;
};

} //namespace godot
