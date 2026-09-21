class_name ExpirintInventoryItem extends InventoryItem

@export_range(0, 100, 1, "prefer_slider", "suffix:s")
var expiration_time: float = 30:
	set(value):
		expiration_time = value
		time_left = expiration_time
		
signal time_left_set_to(time_left)
		
var time_left: float:
	set(value):
		time_left = value
		time_left_set_to.emit(time_left)

func _item_process(delta):
	time_left -= delta * 1
