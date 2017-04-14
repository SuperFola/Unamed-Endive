Objects specification
=====================
An object is defined as follows :

* a unique id (integer)
* a name (string)
* a description (optionnal) (string)
* a price (integer)
* does one use equal one object ? (boolean)
* can we drop it ? (boolean)
* a value (integer)
* a type (integer, translated into an enumeration element)

Each of these will be defined below.

## Unique id
This id is useful to identify easily an object, because they are all stored in the bag as follows :
pocket : [{id: id0, quantity: qu0}, ... {id: idN, quantity: quN}]

## Name
Only interesting to display its information in the bag view.

## Description
Same as before.

## Price
The price is an integer between 0 and 999. A value of 0 does not mean it is free, but that we can not sell it.

## Limited use
If set to false, it is like a bicycle, you can use it how many times you want. Otherwise, one use = one object.

## Throwaway
If set to false, you can not drop the object (example of a rare object). Otherwise you can separate yourself from it.

## Type
We are explaining this field before the value, because it is strongly linked to it.

There is 8 different types of object :

* player (0)
* healpv (1)
* healpp (2)
* healstatus (3)
* capture (4)
* levelup (5)
* lowercooldown (6)
* none (7)

An object of type player is an equipment, as a bicycle.

An object of type healpv if healing the life of a single creature.

An object of type heal pp is healing the power points (no joke) of a creature.

An object of type heal status is taking off a single status of a single creature.

An object of type capture is like a "ball" in the traditional Pokémon games (copyright Nintendo and Game Freak).

An object of type levelup can increase the level of a single creature.

An object of type lowercooldown can lower the cooldown time of a creature.

An object of type none should never be used, it is an internal type of object used when this data can not be found.

## Value
Linked to the type of the object.

The value is an integer between -999 and 999.

Values depending on the type, and effects listed below as described :

```
* [type] :
    * [value] : [effect]
    * [value N] : [effect N]
```

* player :
    * X : the speed of the player is now of X
* healpv :
    * -1 : the creature is fully healed (if not KO only)
    * -2 : the creature is fully healed (if KO only)
    * -3 : the creature is half healed (if KO only)
    * X > 0 : the creature receives X health points
* healpp :
    * -1 : the creature's pp are fully healed
    * X > 0 : the creature receives X pp
* healstatus :
    * 0 : heals the status "burned"
    * 1 : heals the status "paralysed"
    * 2 : heals the status "poisoned"
    * X > 2 : heals all the status
* capture :
    * X : 0 <= X <= 100
         a number between 0 and 100 is chose randomly and compared to the value of the ball. if the number is above or equal
            to the value of the ball, the creature is captured
         if X == 0, it is like a master ball. the higher X gets, the weaker the ball is
* levelup :
    * X : level-up the creature of X levels
* lowercooldown :
    * -1 : the cooldown of a creature is set to 0
    * X > 0 : the creature's cooldown is lowered by X













