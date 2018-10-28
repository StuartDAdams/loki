#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace moka
{
	class transform
	{
		glm::vec3 position_;
		glm::vec3 scale_;
		glm::quat rotation_;

		//transform* parent_;
	public:
		transform(
			const glm::vec3& position
			, const glm::vec3& scale
			, const glm::quat& rotation)
			: position_(position)
			, scale_(scale)
			, rotation_(glm::normalize(rotation))
		{}

		transform()
			: position_{ 0.0f, 0.0f, 0.0f }
			, scale_{ 1.0f, 1.0f, 1.0f }
			, rotation_{ 1.0f, glm::vec3() }
		{}

		transform(const transform& rhs) = default;
		transform& operator = (const transform& rhs) = default;

		transform(transform&& rhs) noexcept = default;
		transform& operator = (transform&& rhs) = default;

		~transform() = default;

		const glm::vec3& get_position() const
		{
			return position_;
		}

		const glm::vec3& get_scale() const
		{
			return scale_;
		}

		const glm::quat& get_rotation() const
		{
			return rotation_;
		}

		glm::mat4 to_matrix() const
		{
			const auto translate = glm::translate(glm::mat4(), get_position());
			const auto rotate = glm::mat4_cast(get_rotation());
			const auto scale = glm::scale(glm::mat4(), get_scale());

			return translate * rotate * scale;
		}

		operator glm::mat4() const
		{
			return to_matrix();
		}

		void set_position(const glm::vec3& position)
		{
			position_ = position;
		}

		void set_scale(const glm::vec3& scale)
		{
			scale_ = scale;
		}

		void set_rotation(const glm::quat& rotation)
		{
			rotation_ = glm::normalize(rotation);
		}

		static glm::vec3 world_front()
		{
			return glm::vec3(0, 0, 1);
		}

		static glm::vec3 world_back()
		{
			return -world_front();
		}

		static glm::vec3 world_up()
		{
			return glm::vec3(0, 1, 0);
		}

		static glm::vec3 world_down()
		{
			return -world_up();
		}

		static glm::vec3 world_right()
		{
			return glm::vec3(1, 0, 0);;
		}

		static glm::vec3 world_left()
		{
			return -world_right();
		}

		static glm::vec3 world_origin()
		{
			return glm::vec3(0, 0, 0);
		}

		glm::vec3 front() const
		{
			return glm::conjugate(rotation_) * world_front();
		}

		glm::vec3 back() const
		{
			return glm::conjugate(rotation_) * world_back();
		}

		glm::vec3 right() const
		{
			return glm::conjugate(rotation_) * world_right();
		}

		glm::vec3 left() const
		{
			return glm::conjugate(rotation_) * world_left();
		}

		glm::vec3 up() const
		{
			return glm::conjugate(rotation_) * world_up();
		}

		glm::vec3 down() const
		{
			return glm::conjugate(rotation_) * world_down();
		}

		void rotate_around(const glm::vec3& point, const glm::quat& rotation)
		{
			auto diff = position_ - point;
			diff = rotation * diff;
			position_ = point + diff;
		}

		void rotate_around(const glm::vec3& point, const glm::vec3& axis, const float angle)
		{
			rotate_around(point, glm::angleAxis(angle, axis));
		}

		void look_at(const glm::vec3& world_location, const glm::vec3& world_up = transform::world_up())
		{
			rotation_ = glm::quat_cast(glm::lookAt(-position_, world_location, world_up));
		}
	};
}