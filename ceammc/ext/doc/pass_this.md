[pass.this]
================

pass thru values equal to given in arguments

.. pd::
    :align: center

    [floatatom] [1 2 3 4 5 6(    [a b c d(
    |           |                |
    [pass.this 2 4 5]            [pass.this b c]
    |                            |
    [prepend set]                [prepend set]
    |                            |
    [   (                        [ (


arguments
-----------------------------
passed values

inlets
------------------
1. input values
    **type**: *float*, *list*, *symbol*


outlet
------------------
1. passed values


.. versionadded:: 0.0.1

.. seealso:: :doc:`pass_if`


