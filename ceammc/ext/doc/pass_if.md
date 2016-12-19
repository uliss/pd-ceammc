[pass.if]
================

conditional pass values according to **predicate** object.

**predicate** should accept value and ouput 0 or 1, for value passing.

.. pd::
    :align: center

    [floatatom] [1 2 3 4 5 6(
    |           |
    |        ___/
    [pass.if]     x [is_odd] /*predicate object, that passes only odd numbers*/
    |
    [prepend set]
    |
    [   (


inlets
------------------

1. input values
    **type**: *float*, *list*, *symbol*
2. gate value from **predicate**
    **type**: *float*

    **values**: 0 (reject) or 1 (pass)


outlets
------------------

1. passed values
2. value to **predicate** object


.. versionadded:: 0.0.1

.. seealso:: :doc:`pass_this`


