#pragma once

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "../resource/inventory.hpp"

namespace godot {

class InventoryHolder : public Node {
	GDCLASS(InventoryHolder, Node)

private:
	Ref<Inventory> inventory;

protected:
	static void _bind_methods();

public:
    void set_inventory(const Ref<Inventory> &p_inventory);
    Ref<Inventory> get_inventory() const;
};

} //namespace godot
