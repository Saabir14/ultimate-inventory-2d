class_name RotatingInventoryItem extends InventoryItem

@export var rotation: float = 10

var angle: float:
	set(value):
		angle = value
		emit_changed()

func _item_process(delta):
	angle += delta
