Exporting
=========

Commands for exporting are located under the **File** menu.

- :ref:`Exporting the Display as a PNG <exporting_the_display_as_a_png>`
- :ref:`Exporting an Animation <exporting_an_animation>`

.. - Printing TODO

.. _exporting_the_display_as_a_png:

""""""""""""""""""""""""""""""
Exporting the Display as a PNG
""""""""""""""""""""""""""""""

MvMf6 can export a PNG (Portable Network Graphics) of the display area.

#. Resize the display area to the desired dimensions.
#. Click the **File** menu and select **Export As PNG**. (:ref:`See note 1 below.<notes_exporting_the_display_as_a_png>`)
#. Select **Screen** if you want a PNG at the screen resolution. Otherwise, select **150 ppi** or **300 ppi** and specify either the **width** or **height** (but not both) of the PNG in inches. (:ref:`See note 2 below.<notes_exporting_the_display_as_a_png>`)
#. Click **OK**.
#. Navigate to the folder in which you want to save the PNG file.
#. In the **File Name** box, enter the name of the PNG file. This name should end with the extension **.png**
#. Click Save.

.. _notes_exporting_the_display_as_a_png:
.. note::
    #. The exported PNG has a color depth of 24 bits per pixel, or a maximum of about 16 million colors.
    #. If you select **150 ppi** or **300 ppi** (pixels per inch) in Step 2, the PNG size will depend on the specified **width** or **height** in inches. If you specify the **width**, then the height will be automatically determined so that the aspect ratio of the PNG matches that of the display area. If you specify **height**, then the width will be automatically determined. For example, if you specify 300 ppi and a width of 4 inches, the PNG will have 1200 pixels in the x dimension. When you import such a PNG into an application that recognizes PNG resolution, the PNG will be displayed as a picture with the specified width and height in inches.


.. _exporting_an_animation:

""""""""""""""""""""""
Exporting an Animation
""""""""""""""""""""""

MvMf6 can export a sequence of PNG (Portable Network Graphics) files, one for each time when results are saved. The saved PNG files may be used to create an animation (for example, mpeg, animated gif) using an application that creates animations. (See Note 1 below)

#. :ref:`Resize the display area <setting_size_of_display_area>` to the desired dimensions. (:ref:`See Note 2 below <notes_exporting_an_animation>`)
#. Click the **File** menu and select **Export Animation**.
#. Set the **Animation type** to "time" or "viewpoint."
#. If the **Animation type** is set to "viewpoint", set the **Number of steps**.
#. If the **Animation type** is set to "time", specify the time span in the **From** and **To** boxes.
#. In the **Rotate box**, enter the amount of viewpoint rotation (in degrees) between successive times. (:ref:`See Note 3 below <notes_exporting_an_animation>`)
#. In the **Elevate** box, enter the amount of viewpoint elevation (in degrees) between successive times. (:ref:`See Note 3 below <notes_exporting_an_animation>`)
#. Enter the **File Prefix** and the **Start Number**. For example, if you enter **sim** for File Prefix and **001** for Start Number, then the files will be named sim001.png, sim002.png, sim003.png, etc.
#. Click the **Browse** button and navigate to the folder in which you want to save the files. Click the folder and then click **Select**.
#. To preview the animation that will be saved, click **Preview**. The program will display the sequence of images to be saved, but will not save them.
#. To save the animation, click **Export**. A sequence of PNG files are created.
#. Click **Save**.

.. _notes_exporting_an_animation:
.. note::
    #. The PNG files are saved at the screen resolution and have the same size as the :ref:`display area <display_area>`. The color depth is 24 bits per pixel, or a maximum of about 16 million colors.
    #. To conserve disk space, :ref:`resize the display area <setting_size_of_display_area>` to a relatively small size before saving the sequence of PNG files.
    #. If you want a stationary viewpoint for the animation, enter 0 in both the **Rotate** and **Elevate** boxes.
