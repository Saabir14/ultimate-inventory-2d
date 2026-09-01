#pragma once

#include "godot_cpp/classes/node.hpp"
#include "resource/inventory_item.hpp"


// ItemNode holds an item
// Handles instantiating the UI scene of the item
namespace godot {

class ItemUiNode : public Node {
	GDCLASS(ItemUiNode, Node)

private:
	Ref<InventoryItem> item;

protected:
	static void _bind_methods();

public:
	void set_item(const Ref<InventoryItem> &p_item);
	Ref<InventoryItem> get_item() const;
};
} //namespace godot
