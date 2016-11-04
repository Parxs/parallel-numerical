Interpolation with the used algorithm is not really useful to parallelize as 
1) It works only up to degree 13 and therefore only with 14 x_i
2) The part of the code which takes the most time is not parallelizable
    because each loop iteration depends on the one before it
    
    
This is only kept for completeness sake and for the unlikely scenario
that one of its (helper-) functions may be usable else where.
