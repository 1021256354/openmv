/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * MicroPython helper functions.
 *
 */
#include "py_helper.h"

#ifdef __CC_ARM
__WEAK bool rectangle_overlap(rectangle_t *ptr0, rectangle_t *ptr1)
{
    int x0 = ptr0->x;
    int y0 = ptr0->y;
    int w0 = ptr0->w;
    int h0 = ptr0->h;
    int x1 = ptr1->x;
    int y1 = ptr1->y;
    int w1 = ptr1->w;
    int h1 = ptr1->h;
    return (x0 < (x1 + w1)) && (y0 < (y1 + h1)) && (x1 < (x0 + w0)) && (y1 < (y0 + h0));
}

__WEAK void rectangle_intersected(rectangle_t *dst, rectangle_t *src)
{
    int leftX = IM_MAX(dst->x, src->x);
    int topY = IM_MAX(dst->y, src->y);
    int rightX = IM_MIN(dst->x + dst->w, src->x + src->w);
    int bottomY = IM_MIN(dst->y + dst->h, src->y + src->h);
    dst->x = leftX;
    dst->y = topY;
    dst->w = rightX - leftX;
    dst->h = bottomY - topY;
}

#endif

int py_helper_keyword_int(uint n_args, const mp_obj_t *args, uint arg_index,
                          mp_map_t *kw_args, mp_obj_t kw, int default_val)
{
    mp_map_elem_t *kw_arg = mp_map_lookup(kw_args, kw, MP_MAP_LOOKUP);

    if (kw_arg) {
        default_val = mp_obj_get_int(kw_arg->value);
    } else if (n_args > arg_index) {
        default_val = mp_obj_get_int(args[arg_index]);
    }

    return default_val;
}

float py_helper_keyword_float(uint n_args, const mp_obj_t *args, uint arg_index,
                              mp_map_t *kw_args, mp_obj_t kw, float default_val)
{
    mp_map_elem_t *kw_arg = mp_map_lookup(kw_args, kw, MP_MAP_LOOKUP);

    if (kw_arg) {
        default_val = mp_obj_get_float(kw_arg->value);
    } else if (n_args > arg_index) {
        default_val = mp_obj_get_float(args[arg_index]);
    }

    return default_val;
}

void py_helper_keyword_int_array(uint n_args, const mp_obj_t *args, uint arg_index,
                                 mp_map_t *kw_args, mp_obj_t kw, int *x, int size)
{
    mp_map_elem_t *kw_arg = mp_map_lookup(kw_args, kw, MP_MAP_LOOKUP);

    if (kw_arg) {
        mp_obj_t *arg_array;
        mp_obj_get_array_fixed_n(kw_arg->value, size, &arg_array);
        for (int i = 0; i < size; i++) x[i] = mp_obj_get_int(arg_array[i]);
    } else if (n_args > arg_index) {
        mp_obj_t *arg_array;
        mp_obj_get_array_fixed_n(args[arg_index], size, &arg_array);
        for (int i = 0; i < size; i++) x[i] = mp_obj_get_int(arg_array[i]);
    }
}

void py_helper_keyword_float_array(uint n_args, const mp_obj_t *args, uint arg_index,
                                   mp_map_t *kw_args, mp_obj_t kw, float *x, int size)
{
    mp_map_elem_t *kw_arg = mp_map_lookup(kw_args, kw, MP_MAP_LOOKUP);

    if (kw_arg) {
        mp_obj_t *arg_array;
        mp_obj_get_array_fixed_n(kw_arg->value, size, &arg_array);
        for (int i = 0; i < size; i++) x[i] = mp_obj_get_float(arg_array[i]);
    } else if (n_args > arg_index) {
        mp_obj_t *arg_array;
        mp_obj_get_array_fixed_n(args[arg_index], size, &arg_array);
        for (int i = 0; i < size; i++) x[i] = mp_obj_get_float(arg_array[i]);
    }
}

int py_helper_lookup_int(mp_map_t *kw_args, mp_obj_t kw, int default_val)
{
    mp_map_elem_t *kw_arg = mp_map_lookup(kw_args, kw, MP_MAP_LOOKUP);

    if (kw_arg != NULL) {
        default_val = mp_obj_get_int(kw_arg->value);
    }
    return default_val;
}

float py_helper_lookup_float(mp_map_t *kw_args, mp_obj_t kw, float default_val)
{
    mp_map_elem_t *kw_arg = mp_map_lookup(kw_args, kw, MP_MAP_LOOKUP);

    if (kw_arg != NULL) {
        default_val = mp_obj_get_float(kw_arg->value);
    }
    return default_val;
}

void py_helper_lookup_int_array(mp_map_t *kw_args, mp_obj_t kw, int *x, int size)
{
    mp_map_elem_t *kw_array = mp_map_lookup(kw_args, kw, MP_MAP_LOOKUP);

    if (kw_array != NULL) {
        mp_obj_t *arg_array;
        mp_obj_get_array_fixed_n(kw_array->value, size, &arg_array);
        for (int i=0; i<size; i++) {
            x[i] = mp_obj_get_int(arg_array[i]);
        }
    }
}

void py_helper_lookup_float_array(mp_map_t *kw_args, mp_obj_t kw, float *x, int size)
{
    mp_map_elem_t *kw_array = mp_map_lookup(kw_args, kw, MP_MAP_LOOKUP);

    if (kw_array != NULL) {
        mp_obj_t *arg_array;
        mp_obj_get_array_fixed_n(kw_array->value, size, &arg_array);
        for (int i=0; i<size; i++) {
            x[i] = mp_obj_get_float(arg_array[i]);
        }
    }
}

int py_helper_lookup_color(mp_map_t *kw_args, int default_color)
{
    mp_map_elem_t *kw_color = mp_map_lookup(kw_args,
            MP_OBJ_NEW_QSTR(MP_QSTR_color), MP_MAP_LOOKUP);

    if (kw_color != NULL) {
        if (mp_obj_is_integer(kw_color->value)) {
            default_color = mp_obj_get_int(kw_color->value);
        } else {
            mp_obj_t *arg_color;
            mp_obj_get_array_fixed_n(kw_color->value, 3, &arg_color);
            default_color = IM_RGB565(IM_R825(mp_obj_get_int(arg_color[0])),
                                      IM_G826(mp_obj_get_int(arg_color[1])),
                                      IM_B825(mp_obj_get_int(arg_color[2])));
        }
    }
    return default_color;
}

void py_helper_lookup_offset(mp_map_t *kw_args, image_t *img, point_t *p)
{
    mp_map_elem_t *kw_offset = mp_map_lookup(kw_args,
            MP_OBJ_NEW_QSTR(MP_QSTR_offset), MP_MAP_LOOKUP);

    if (kw_offset == NULL) {
        p->x = 0;
        p->y = 0;
    } else {
        mp_obj_t *arg_offset;
        mp_obj_get_array_fixed_n(kw_offset->value, 2, &arg_offset);
        p->x = mp_obj_get_int(arg_offset[0]);
        p->y = mp_obj_get_int(arg_offset[1]);
    }
}

void py_helper_lookup_rectangle(mp_map_t *kw_args, image_t *img, rectangle_t *r)
{
    py_helper_lookup_rectangle_2(kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_roi), img, r);
}

void py_helper_lookup_rectangle_2(mp_map_t *kw_args, mp_obj_t kw, image_t *img, rectangle_t *r)
{
    mp_map_elem_t *kw_rectangle = mp_map_lookup(kw_args, kw, MP_MAP_LOOKUP);

    if (kw_rectangle == NULL) {
        r->x = 0;
        r->y = 0;
        r->w = img->w;
        r->h = img->h;
    } else {
        mp_obj_t *arg_rectangle;
        mp_obj_get_array_fixed_n(kw_rectangle->value, 4, &arg_rectangle);
        r->x = mp_obj_get_int(arg_rectangle[0]);
        r->y = mp_obj_get_int(arg_rectangle[1]);
        r->w = mp_obj_get_int(arg_rectangle[2]);
        r->h = mp_obj_get_int(arg_rectangle[3]);

        if ((r->w <= 0) || (r->h <= 0)) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_AssertionError, "Invalid ROI dimensions!"));
        }

        rectangle_t tmp;
        tmp.x = 0;
        tmp.y = 0;
        tmp.w = img->w;
        tmp.h = img->h;

        if (!rectangle_overlap(r, &tmp)) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_AssertionError, "ROI does not overlap the image!"));
        }

        rectangle_intersected(r, &tmp);
    }

    if ((r->w <= 0) || (r->h <= 0)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_AssertionError, "Invalid ROI dimensions!"));
    }
}
