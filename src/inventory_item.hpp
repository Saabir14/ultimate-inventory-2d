#pragma once

#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/resource.hpp"

namespace godot {

class InventoryItem : public Resource {
	GDCLASS(InventoryItem, Resource)

private:
	Ref<PackedScene> item_ui_scene;
	Ref<PackedScene> item_2d_scene;

protected:
	static void _bind_methods();

public:
	void set_item_ui_scene(const Ref<PackedScene> &p_scene);
	Ref<PackedScene> get_item_ui_scene() const;

	void set_item_2d_scene(const Ref<PackedScene> &p_scene);
	Ref<PackedScene> get_item_2d_scene() const;
};

} //namespace godot
