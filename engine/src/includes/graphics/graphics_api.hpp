#pragma once

#include <ostream>
#include <maths/colour.hpp>
#include <maths/vector3.hpp>
#include <asset_importer/texture_importer.hpp>
#include <vector>
#include <utility>

namespace moka
{
    enum class attribute
    {
        position,  
        normal,    
        tangent,   
        bitangent, 
        color0,    
        color1,  
        color2,    
        color3,    
        indices,   
        weight,    
        tex_coord0, 
        tex_coord1, 
        tex_coord2, 
        tex_coord3, 
        tex_coord4, 
        tex_coord5, 
        tex_coord6, 
        tex_coord7,
        extension
    };

    enum class attribute_type
    {
        int8,   
        int16,
        int32,
        int64,
        uint8,
        uint16,
        uint32,
        uint64,
        float16,
        float32,
        float64,
        boolean,
    };

    inline size_t attr_type_size(const attribute_type attr_type)
    {
        switch (attr_type)
        {
        case attribute_type::int8:
            return sizeof(int8_t);
        case attribute_type::int16:
            return sizeof(int16_t);
        case attribute_type::int32:
            return sizeof(int32_t);
        case attribute_type::int64:
            return sizeof(int64_t);
        case attribute_type::uint8:
            return sizeof(uint8_t);
        case attribute_type::uint16:
            return sizeof(uint16_t);
        case attribute_type::uint32:
            return sizeof(uint32_t);
        case attribute_type::uint64:
            return sizeof(int64_t);
        case attribute_type::float32:
            return sizeof(float);
        case attribute_type::float64:
            return sizeof(double);
        default:
            return 0;
        }
    }

    struct vertex_layout;

    struct attribute_element
    {
		size_t index;
		attribute_type type;
		size_t size;
		bool normalized;
		size_t stride;
		size_t offset;

        constexpr attribute_element(
			size_t index
		    , attribute_type type
		    , size_t size
		    , bool normalized
			, size_t stride
			, size_t offset) noexcept
            : index(index)
			, type(type)
			, size(size) 
			, normalized(normalized)
			, stride(stride) 
			, offset(offset)
        {}
    };

    
    struct vertex_layout_builder;

    struct vertex_layout
    {
    private:
		std::vector<attribute_element> layout_;
    public:
		vertex_layout() = default;

		vertex_layout(std::vector<attribute_element>&& layout)
			: layout_(std::move(layout))
        {}

		using builder = vertex_layout_builder;

        size_t total_size() const noexcept;

        auto begin() noexcept
        {
            return layout_.begin();
        }

        auto end() noexcept
        {
            return layout_.end();
        }

        auto begin() const noexcept
        {
            return layout_.begin();
        }

        auto end() const noexcept
        {
            return layout_.end();
        }
    };

    struct vertex_layout_builder
    {
    private:
		std::vector<attribute_element> attr;
    public:
        vertex_layout_builder& add_attribute(size_t index, attribute_type type, size_t size, bool normalized, size_t stride, size_t offset);
        vertex_layout build();
    };

    inline size_t vertex_layout::total_size() const noexcept
    {
        size_t result = 0;
        for (const auto& element : layout_)
        {
            result += element.size;
        }
        return result;
    }

    inline vertex_layout_builder& vertex_layout_builder::add_attribute(
		size_t index, attribute_type type, size_t size, bool normalized, size_t stride, size_t offset)
    {
        this->attr.emplace_back(index, type, size, normalized, stride, offset);
        return *this;
    }

    inline vertex_layout vertex_layout_builder::build()
    {
		return vertex_layout{ std::move(attr) };
    }

    template<typename T>
    class basic_rectangle
    {
        T x_, y_, width_, height_;
    public:
        constexpr basic_rectangle(const T x, const T y, const T width, const T height) noexcept
            : x_{ x }, y_{ y }, width_{ width }, height_{ height }
        {}

        constexpr T x() const noexcept
        {
            return x_;
        }

        constexpr T y() const noexcept
        {
            return y_;
        }

        constexpr T width() const noexcept
        {
            return width_;
        }

        constexpr T height() const noexcept
        {
            return height_;
        }
    };

    using rectangle = basic_rectangle<int>;

    enum class primitive_type : uint8_t
    {
		points
		, lines
		, line_loop
		, line_strip
		, triangles
		, triangle_strip
		, triangle_fan
    };

    enum class toggle : uint8_t
    {
        enable,
        disable
    };

    enum class cull_face : uint8_t
    {
        front,
        back,
        front_and_back
    };

    enum class blend_function_factor : uint8_t
    {
        zero,                      
        one,                       
        src_colour,                
        one_minus_src_colour,      
        dst_colour,                
        one_minus_dst_colour,      
        src_alpha,                 
        one_minus_src_alpha,       
        dst_alpha,                 
        one_minus_dst_alpha,       
        constant_colour,           
        one_minus_constant_colour,  
        constant_alpha,            
        one_minus_constant_alpha   
    };

    enum class shader_attribute : uint8_t
    {
        position,   
        normal,     
        tangent,    
        bitangent,  
        color0,     
        color1,     
        color2,     
        color3,     
        indices,    
        weight,     
        tex_coord0, 
        tex_coord1, 
        tex_coord2, 
        tex_coord3, 
        tex_coord4, 
        tex_coord5, 
        tex_coord6, 
        tex_coord7, 
    };

    enum class parameter_type : uint8_t
    {
        texture, //!< sampler
        vec3, //!< 3 floats vector uniform
        vec4, //!< 4 floats vector uniform
        mat3, //!< 3x3 matrix uniform
        mat4, //!< 4x4 matrix uniform
		float32 //! single floating point uniform
    };

    enum class shader_type : uint8_t
    {
        vertex,	  
        fragment, 
        compute   
    };

	enum class face_culling : uint8_t
	{
		front,
		back,
		front_and_back
	};

    using handle_id = uint16_t;

    struct dynamic_index_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct dynamic_vertex_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct frame_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct index_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct indirect_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct occlusion_query_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct program_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct shader_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct texture_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct vertex_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

	template<typename T>
	bool is_handle_valid(const T& handle)
	{
		return handle.id != std::numeric_limits<moka::handle_id>::max();
	}

	struct uniform_data
	{
		std::string name;
		parameter_type type;
		size_t count;
		size_t buffer_start;
		size_t buffer_end;
	};

	struct texture_binding
	{
		texture_handle handle;
		size_t unit;
	};

	struct draw_call;

    /**
     * \brief render_context abstracts the native rendering API.
     */
    class graphics_api
    {
    public:
        virtual ~graphics_api() = default;
		
		virtual program_handle create_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle) = 0;
        virtual shader_handle create_shader(shader_type type, const std::string& source) = 0;
        virtual vertex_buffer_handle create_vertex_buffer(const void* vertices, size_t size, const vertex_layout& decl) = 0;
		virtual index_buffer_handle create_index_buffer(const void* indices, size_t size) = 0;
		virtual texture_handle create_texture(const texture_data& data) = 0;
		
		virtual void submit(draw_call&& call) = 0;

		virtual void frame() = 0;
    };
}
