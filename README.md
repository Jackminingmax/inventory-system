inventory-system
This is a c++ program made in visual studio that allows the user to manage an inventory system.

![Screenshot 2024-11-20 151147](https://github.com/user-attachments/assets/1c6d424f-d914-4631-92f1-ac40955053fe)

It will allow you to create and delete categories of your choice, and add and remove items from those categories.

![Screenshot 2024-11-20 151348](https://github.com/user-attachments/assets/5eab678f-5ef9-4bd5-b29a-3df13b506ed0)

It can automatically generate a SKU number and includes the date the item was added in its description.

![Screenshot 2024-11-20 151643](https://github.com/user-attachments/assets/839a0d12-cfab-4def-8c27-8def6bd53b66)

It automatically keeps all items and categories in order.

![Screenshot 2024-11-20 152353](https://github.com/user-attachments/assets/ed97ab53-7914-4036-811a-488a7bedf5dc)

As you see it can also print out any and all information it has.

You can delete items and categories and the program will take care of reusing those slots and re-ordering everything.

Categories are implemented into an AVL Tree, and the items are inserted into a dynamic array inside the AVL tree.

Categories can currently handle up to 999 items each.

To end the program, just enter 0 at the hub. Exiting any other way will not save the changes you've made.
