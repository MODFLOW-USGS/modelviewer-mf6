.. _toolboxes:

Toolboxes
=========

MvMf6 provides a number of small dialog boxes, called **toolboxes**, for controlling specific aspects of the display. 

- The **Toolbox** menu provides commands to show or hide the toolboxes. 
- A check mark next to a command indicates that the corresponding toolbox is shown. Selecting this command will hide the toolbox. 
- The absence of a check mark next to a command indicates that the corresponding toolbox is hidden. Selecting this command will show the toolbox. 
- Each toolbox may be closed by clicking **Done**. 
- Toolboxes do not have to be closed for the program to continue. 
- Except for text entries, all controls on toolboxes will take effect immediately when changed.
- For text entries, the user must click **Apply** to apply the change. 

""""""""""""
Data Toolbox
""""""""""""

The **Data** toolbox displays the maximum and minimum data values at the current time, and enables you to select the type of scalar data to display.

- If the **Data** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Data**. 
- The **Scalar** tab displays the minimum and maximum scalar values at the current time. You may also select the type of scalar data to display from the drop-list box. 
- The **Vector** tab displays the minimum and maximum magnitude of vectors at the current time. 
- The **Pathline** tab displays the minimum and maximum travel times for all pathlines. 

.. _color_bar_toolbox:

"""""""""""""""""
Color Bar Toolbox
"""""""""""""""""

The **Color Bar** toolbox controls the properties of the :ref:`color bar <color_bar>`.

- If the **Color Bar** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Color Bar**.
- Clicking **Default** will assign default settings to the selected tab. Default settings are given in parentheses below.
- The **Source** tab allows you to specify whether the color bar displays data for the active data set or the pathlines.
- The **Limits** tab allows you to: 

  * Set the scalar value at the blue and red limits. The default settings are: 

    - blue = minimum scalar value at the current time
    - red = maximum scalar value at the current time 

  * Specify a linear color bar by unchecking the **logarithmic scale** option. (This is the default setting.)
  * Specify a logarithmic color bar by checking the **logarithmic scale** option.
  * Interchange the scalar values at the blue and red limits by clicking **Reverse**.

- The **Size** tab allows you to: 

  * Set the **width** of the color bar in pixels. (20)
  * Set the **height** of the color bar in pixels. (200)
  * Set the **offset** distance in pixels from the right edge of the color bar to the right side of the :ref:`display area <display_area>`. (100)

- The **Labels** tab allows you to: 

  * Set the **font size** (in points) of the labels. (14) 
  * Set the **number of labels**. (5) 
  * Set the **precision** (number of significant places) of the labels. (2) 
  * Set the label color to black, gray, or white. (black) 

- The **Colors** tab allows you to: 

  * Select the Default color scheme 
  * Select the Modified color scheme which is similar to the default except that the thickness of the yellow and aqua portions of the color bar have been increased at the expense of the green, red, and blue portions of the color bar. 
  * Select a Custom color scheme in which the beginning and ending colors are selected by the user. 

If the Custom color scheme is selected, the user clicks the **Choose First Color** or **Choose Last Color** buttons to choose the colors of the endpoints of the color bar. 

- The Crop tab allows you to: 

  * Specify the **Min** and **Max** positions of the x, y, and z cropping planes. (Min = 0, Max = 1) 
  * Specify the **Delta** value, which is used to increase or decrease the Min or Max position when the up or down arrow is clicked. (0.1) 
  * Specify the **Crop Angle** to rotate the x and y cropping planes. The crop angle can be from -45 to 45 degrees. (0) 


""""""""""""""""
Lighting Toolbox
""""""""""""""""

The **Lighting** toolbox allows adjustment of how graphic objects are illuminated. The default lighting provided by MvMf6 is a **headlight**, which illuminates objects from the direction of the viewpoint. *The default setting is adequate for most cases, and lighting adjustment is often unnecessary*. 

- If the **Lighting** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Lighting**. 
- Clicking **Default** will assign default settings to the selected tab. Default settings are given in parentheses below. 
- The **Lights** tab allows you to: 

  * Turn on the headlight by checking the **Headlight** option. (By default, the headlight is turned on.) 
  * Turn off the headlight by unchecking the **Headlight** option. 
  * Set the headlight intensity. (1) 
  * Turn on the auxiliary light by checking the **Auxiliary Light** option. 
  * Turn off the auxiliary light by unchecking the **Auxiliary Light** option. (By default, the auxiliary light is turned off.) 
  * Set the auxiliary light intensity. (0) 
  * Set the vector direction from which the auxiliary light illuminates graphic objects. The vector components (x, y, z) may vary from -1 to 1. For example, if x= 0, y= 0, z= 1, the auxiliary light will shine from the positive z axis towards the negativez axis.

- The **Surface** tab allows you to set the surface properties of graphic objects. These properties are:

  * Diffuse: This parameters is currently not adjustable and is set to 1.0.
  * Ambient: Increasing this parameter above 0 gives surfaces a "washed out" appearance and reduces the shading. Setting this parameter to the maximum value of 1 causes surfaces to appear white. (0)
  * Specular: The amount of reflected light. (0)
  * Specular Power: The surface shininess. (1)

- The **Background** tab allows you to:

  * Set the background color to white by selecting the **White** option. (White is the default background color.)
  * Set the background to any color by selecting the **Custom** option and specifying the red, green, and blue components of the color.

.. _grid_toolbox:

""""""""""""
Grid Toolbox
""""""""""""

The **Grid** toolbox controls the display of the model grid.

- If the **Grid** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Grid**.

- The Lines tab allows you to:

  * Set the position of :ref:`gridline set <the_i_j_k_index_system_for_gridlines>` defined by index i, j, or k for gridlines.
  * Click the up or down arrow to increase or decrease the i, j, or k indices. 
  * Show or hide a gridline set by checking or unchecking the **Activate** option. 
  * Set the gridline color to black, gray, or white. 

- The **Shell** tab allows you to set the color (red, green, and blue components) and opacity of the shell that represents the outer surface of the active grid. 
- The **Subgrid** tab allows you to:

  * Check the **Activate Subgrid** option to show a subset of the grid as defined by the range of i, j, and k indices for cells. 
  * Uncheck the **Activate Subgrid** option to show the full grid. 
  * Specify the minimum and maximum values for the i, j, and k indices. 
  * Click the up or down arrow to increase or decrease the i, j, or k indices. 

.. _geometry_toolbox:

""""""""""""""""
Geometry Toolbox
""""""""""""""""

The **Geometry** toolbox controls the geometric aspects of the display.

- If the **Geometry** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Geometry**.
- The **Scale** tab allows you to set the exaggeration or elongation factor in the x, y and z directions. (For no exaggeration, set all factors to 1.)
- The **Axes Symbol** tab allows you to:

  * Display the axes symbol as **tubes** or **lines**.
  * Set the **size** of the axes symbol. (MvMf6 determines a default size of 1. To increase the size, specify a value greater than 1. To decrease the size, specify a value smaller than 1.)
  * Set the **tube diameter** for the axes symbol. (MvMf6 determines a default tube diameter of 1. To increase the diameter, specify a value greater than 1. The decrease the diameter, specify a value smaller than 1.)
  * Set the (x, y, z) position of the axes symbol.

- The **Bounding Box** tab allows you to set the color of the bounding box. 

.. _overlay_toolbox:

"""""""""""""""
Overlay Toolbox
"""""""""""""""

The **Overlay** toolbox is used to import an :ref:`overlay <overlay>` contained in either an ESRI Shapefile or a DXF file.

To import an overlay:

1. If the **Overlay** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting Overlay.
2. Click the **File** tab.
3. Select the **Type** of file (ESRI Shapefile or DXF file) to be imported.
4. Click the **Browse** button and navigate to the file you want to import. Double click the file.
5. Click the **Control** tab.
6. Enter the following information:

  * **X0** and **Y0** are the x and y coordinates in the overlay file (that is, the Shapefile or DXF file) of the point that coincides with the lower left corner of the grid, when the grid is viewed from directly above.
  * **Scale** is the conversion factor that converts the length unit used in the overlay file to the length unit of the model grid. For example, if the overlay file data are in meters, and the grid is in feet, then "scale" should be 3.2808.
  * **Angle** is the angle in degrees to rotate the overlay. A positive value cause rotation in the clockwise direction.

7. Next, select either:

  * **Flat** : The overlay is displayed on a flat plane, or
  * **Drape** : The overlay is displayed on the top of the grid.

8. If you select Flat then:

  * Enter a value for **Elev**, which is the elevation (in length unit used by the grid) of the plane on which the overlay is displayed.
  * If you check the **Trim** option, the overlay will be trimmed so that any portion outside the grid (when viewed from directly above) is removed. If you activate the subgrid (using the Grid Toolbox), then the overlay will be trimmed to the subgrid.
  * If you uncheck the **Trim** option, the entire overlay will be displayed. (Even if you activate the subgrid, the overlay will not be trimmed.)

9. If you select **Drape** then:

  * The **Trim** option is automatically turned on and cannot be turned off. This is necessary because the program doesn't know how to drape the overlay for those portions outside the grid. Therefore, any portion of the overlay outside the grid (or subgrid) is removed.
  * You may want to raise the overlay slightly above the top of the grid. This can be done by specifying the **drape gap**. When the drape gap is zero, the overlay is exactly on the top surface of the grid. If you set the drape gap to 1.5, the overlay is 1.5 length units (whatever length unit used in the model) above the top surface of the grid. (In solid display, the overlay should be slightly above the top of the grid so that the grid does not partially cover the overlay.)

10. If desired, select **Crop**:

  * If the you click the **Crop** option, portions of the overlay are removed so that the overlay appears only above the cropped solid.

11. Click the **Apply** button.

.. _solid_toolbox:

"""""""""""""
Solid Toolbox
"""""""""""""

The **Solid** toolbox controls the display of scalar data as a :ref:`solid <solid>`.

- If the Solid toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Solid**.
- To specify the :ref:`coloring scheme <coloring_schemes>`, select one of the options: **Blocky**, **Smooth**, or **Banded**. (The Blocky coloring scheme is not available for finite-element models such as SUTRA.)
- For the banded coloring scheme, you may set the number of color bands in the text box.
- To apply :ref:`thresholding <thresholding>`:

  * Check the Apply Threshold option.
  * Specify the minimum and maximum values of the threshold range.
  * Click **Apply**.

- The **Full** button sets the minimum and maximum values of the threshold range respectively to the minimum and maximum values of the scalar data at the current time step. 
- To turn off :ref:`thresholding <thresholding>`, uncheck the **Apply Threshold** option.

.. _isosurface_toolbox:

""""""""""""""""""
Isosurface Toolbox
""""""""""""""""""

The **Isosurface** toolbox is used to create :ref:`isosurfaces <isosurface>`.

- If the **Isosurface** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Isosurface**.
- The **Regular** tab allows you to create isosurfaces at scalar values uniformly spaced between a minimum and a maximum value. For example, if Min Value = 0, Max Value = 50, No. of Isosurfaces = 6, then isosurfaces will be created for scalar values of 0, 10, 20, 30, 40, and 50.
- The **Custom** tab allows you to create isosurfaces for arbitrary scalar values. These values are shown in the list box on the right.

  * To add an isosurface, enter the scalar value in the text box on the left and click **Add**.
  * To delete an isosurface, select the scalar value in the list box on the right and click **Delete**.

.. _vector_toolbox:

""""""""""""""
Vector Toolbox
""""""""""""""

The Vector toolbox controls the :ref:`display of vector data <vector_data>`.

- If the **Vector** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Vector**.
- Clicking **Default** will assign default settings to the selected tab. Default settings are given in parentheses below.
- The **Subsample** tab allows you to display a subset of vectors.

  * The subset of vectors is defined by the minimum value (Min), maximum value (Max), and the sampling rate for the i, j, and k indices. For example, if Min = 3, Max = 20, and Rate = 4 for index i, then vectors will be displayed at i values of 3, 7, 11, 15 and 19. (By default, Min, Max and Rate are set to show all vectors.)
  * You may also click the up or down arrow to increase or decrease the Min or Max values. For SUTRA models with irregular meshes, only the k rate is active and the nodes throughout the mesh will be subampled based on their node numbers.

- The **Appearance** tab allows you to: 

  * Set the **Scale Factor** for the vectors. (:ref:`See note 1 below. <vector_toolbox_notes>`) Check of uncheck the Log Transform option to either apply a log transformation to the ratio of the vector to the smallest vector or to use the vector magnitudes without transformation.
  * Check or uncheck the **Show Base** option to show or hide small cubes at the starting point of each vector.
  * Check the up or down arrow to increase or decrease the size of the base cubes.
  * Set the vector color to black, gray, or white. (black)

- The Threshold tab allows you to:

  * Check the **Apply Threshold** option to display only those vectors having magnitudes within the threshold range.
  * Uncheck the **Apply Threshold** option to turn off thresholding.
  * Specify the minimum and maximum vector magnitudes to define the threshold range.

- The Crop tab allows you to:

  * Select which vectors are displayed using a :ref:`cropping procedure similar to that used to crop isosurfaces or solids <crop_toolbox>`.

.. _vector_toolbox_notes:

.. note::
    #. When model data are loaded, MvMf6 computes a default scale factor based on the maximum vector magnitude at the selected time to display data. The actual value of the scale factor is unimportant. To lengthen the vectors, simply increase the scale factor. To shorten the vectors, decrease the scale factor.

.. _model_features_toolbox:

""""""""""""""""""""""
Model Features Toolbox
""""""""""""""""""""""

The **Model Features** toolbox controls the :ref:`display of model features <model_features>`.

- If the **Model Features** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Model Features**.
- The **Model Features** toolbox contains two lists: the **Show** list on the left and the **Hide** list on the right.
- Model features in the Show list are visible; those in the Hide list are invisible.
- To show a model feature in the Hide list, select the item and click **Show**.
- To hide a model feature in the Show list, select the item and click the **Hide**.
- If several model features occupy the same cell or node, the uppermost item in the Show list will be displayed.
- The position of a model feature in the Show list may be changed by selecting the item and then clicking

  * **Top**: Move the item to the top of the list
  * **Up**: Move the item up one position
  * **Down**: Move the item down one position
  * **Bot**: Move the item to the bottom of the list

- To change the color of a model feature, select the item, click **Color**, and then select the color. 
- In finite element meshes, to change the size of a model feature click the arrows next to **Size**. 

.. _crop_toolbox:

""""""""""""
Crop Toolbox
""""""""""""

The **Crop** toolbox is used for :ref:`cropping solid or isosurfaces <cropping>`.

- If the **Crop** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Crop**.
- Clicking **Default** will assign default settings to the selected tab. Default settings are given in parentheses below.
- The **Controls** tab allows you to:

  * Specify the **Min** and **Max** positions of the x, y, and z cropping planes. (Min = 0, Max = 1)
  * Specify the **Delta** value, which is used to increase or decrease the Min or Max position when the up or down arrow is clicked. (0.1)
  * Check or uncheck the option **Min=Max**. When checked, the Min position is set equal to the Max position. When the up or down arrow is clicked to change Min (or Max) position, the Max (or Min) position is also changed by the same amount.
  * Specify the **Crop Angle** to rotate the x and y cropping planes. The crop angle can be from -45 to 45 degrees. (0)   

- The **Cropped-Away Pieces** tab allows you to:

  * Check or uncheck the Show to show or hide the cropped-away pieces. (By default, the cropped-away pieces are not shown.)
  * Set the color (red, green, and blue components) and the opacity of the cropped-away pieces. (The default settings are: red = 1, green = 0.8, blue = 0.6, opacity = 0.2.)

"""""""""""""""""
Animation Toolbox
"""""""""""""""""

The **Animation** toolbox controls the display of model results at successive times to create an animation.

- If the **Animation** toolbox is not visible, display it by clicking the **Toolbox** menu and selecting **Animation**.
- The **Controls** tab allows you to do the following:

  * To choose whether the animation will advance through time or change the viewpoint without advancing through time, set the **Animation type** to "time" or "viewpoint."
  * If the animation type is set to "viewpoint" you can set the **Number of steps** in the animation.
  * To advance to the next time or position, click **Advance**.
  * To run an animation (continuously advance to the next time), click **Run**.
  * To stop a animation, click **Stop**.
  * To set the display to a particular time, select that time from the drop-list box, and then click **Set**.

- The **Options** tab allows you to do the following:

  * Specify a **Delay** (in seconds) between successive times. This will slow down the animation. To animate at the fastest speed, specify a delay of zero.
  * Specify an angle increment to **Rotate** (horizontally) the viewpoint between between successive times.
  * Specify an angle increment to **Elevate** (vertically) the viewpoint between between successive times.
  * For a stationary viewpoint during animation, enter 0 in the Rotate and Elevate text boxes.
