#ifndef BASIC_JPEG_ADAPTER_HPP
#define BASIC_JPEG_ADAPTER_HPP

#include <cstdio>
#include <iostream>
#include <new>
#include <memory>
#include <string>
#include <cstdlib>
#include "jpeglib.h"

#pragma warning( disable : 26812)

class image_data 
{
public:

    int height;
    int width;
    int color_components;
    JSAMPLE* buff;
    J_COLOR_SPACE color_space;
    
    image_data() : buff{ nullptr }, height{ 0 }, width{ 0 }, color_components{ 0 }, color_space { J_COLOR_SPACE::JCS_UNKNOWN } { }

    image_data(int w, int h, int cc, J_COLOR_SPACE cs) : width{ w }, height{ h }, color_components{ cc }, color_space{ cs }
    {
        allocate_memory();
    }

    image_data(const image_data& im_data) : width{ im_data.width }, height{ im_data.height }, color_components{ im_data.color_components }, color_space{ im_data.color_space }
    {
        allocate_memory();
        fill_buffer(im_data.buff);
    }
    
    image_data& operator=(const image_data& rhs) 
    {
        if(this != &rhs) 
        {
            update(rhs.width, rhs.height, rhs.color_components, rhs.color_space);
            fill_buffer(rhs.buff);
        }
        return *this;
    }
    
    virtual ~image_data() 
    {
        delete_buffer();
    }
    
    void fill_buffer(unsigned int val) 
    {
        if(val > 255) 
        {
            val = 255;
        }
        std::raw_storage_iterator<JSAMPLE*, JSAMPLE> raw_it(buff);
        for(int i = 0; i < height * width * color_components; ++i, ++raw_it)
            *raw_it = val;  
    }

    void fill_buffer(unsigned int r, unsigned int g, unsigned int b) 
    {
        if( r > 255)
            r = 255;
        
        if( g > 255)
            g = 255;
        
        if( b > 255)
            b = 255;
        
        std::raw_storage_iterator<JSAMPLE*, JSAMPLE> raw_it(buff);
        for(int i = 0; i < height * width * color_components; ++i, ++raw_it) 
        {
            if(i % 3 == 0)
                *raw_it = r;    
            else if(i % 3 == 1)
                *raw_it = g;
            else
                *raw_it = b;
        }
    }

    void update(int w, int h, int cc, J_COLOR_SPACE cs) 
    {
        delete_buffer();
        update_parameters(w, h, cc, cs);
        allocate_memory();
    }

    void clear()
    {
         delete_buffer();
    }

    void print_buffer() 
    {
        for(int i = 0; i < height * width * color_components; ++i)
            std::cout << (int)buff[i] << " ";
        
        std::cout << std::endl;
    }

private:

    void update_parameters(int w, int h, int cc, J_COLOR_SPACE cs)
    {
        width = w;
        height = h;
        color_components = cc;
        color_space = cs;
    }

    void allocate_memory()
    {
        buff = (JSAMPLE*) operator new(sizeof(JSAMPLE) * height * width * color_components, std::nothrow);
        if(!buff) 
        {
            std::cerr << "Memory allocation error!" << std::endl;
            exit(1);
        }
    }

    void fill_buffer(const JSAMPLE* const _buff) 
    {
        for(int i = 0; i < height * width * color_components; ++i) 
            buff[i] = _buff[i];
    }

    void delete_buffer() 
    {
        if(buff) 
        {
            delete buff;
            buff = nullptr;
        }
    }
};

void write_jpeg_file(std::string filename, int quality, const image_data& im_data);
void read_JPEG_file (std::string filename, image_data& im_data);

#endif // BASIC_JPEG_ADAPTER_HPP
