/*
 * Copyright 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_VAOPROGRAMINPUT_HPP
#define ENGINE_VAOPROGRAMINPUT_HPP

#include <glcxx/src/vao.hpp>

/// @brief named attribute class, used for program declarations
template<typename TName, typename TAttribTraits>
struct TNamedAttrib
{
      using tName = TName;
      using tAttribTraits = TAttribTraits;
};

/// @brief holds state of program's vao
template<typename TNamedAttribTuple> class TVertexArrayObjectProgramInput;

/// @brief specialization for tuple
template<typename... TNamedAttribs>
class TVertexArrayObjectProgramInput<std::tuple<TNamedAttribs...>>
{
      /// @brief return true if Tuple doesn't contain T
      template<typename T, typename Tuple> struct doesnt_contain
      {
            static constexpr bool value = !ct::tuple_contains<Tuple, T>::value;
      };

   public:
      /// @brief vao holds VBOs, which are always vertex shader inputs
      using decl_tag = tag::vertex;

      /// @brief ctstring containing glsl declaration of attributes
      using declaration = ct::string_cat<typename TNamedAttribs::tAttribTraits::template declaration<typename TNamedAttribs::tName>...>;

      /// @brief constructor
      TVertexArrayObjectProgramInput(const GLuint program)
         : mLocations{{glsl::get_attrib_loc(program, TNamedAttribs::tName::chars)...}}
         , mProgramID(program)
      {}

      /// @brief called after program was selected, nothing to do as vao gets
      /// passed to draw function
      void select() const
      {}

      /// @brief draw using index buffer from vao
      template<typename...TName, typename...TData>
      void drawElements(const indexed_vao<ct::named_type<TName, TData>...>& vao) const
      {
         using tVAOTuple = std::tuple<ct::named_type<TName, TData>...>;
         using tRequiredVAOTuple =  std::tuple<ct::named_type<typename TNamedAttribs::tName,
                                                              typename TNamedAttribs::tAttribTraits::data>...>;
         static_assert(!ct::tuple_any_of<tRequiredVAOTuple, doesnt_contain, tVAOTuple>::value, "not all or not matching type inputs for program was provided by given vao");

         vao.bind();
         // if was attached to different program or wasn't attached at all
         if (mProgramID != vao.mProgramID)
         {
            Log::msg("attaching vao to a program id ", mProgramID);
            vao.mProgramID = mProgramID;
            vao.template bind_buffer<cts("indices")>();
            swallow(vao.template bind_buffer<typename TNamedAttribs::tName>() &&
                    (TNamedAttribs::tAttribTraits::attach(mLocations[ct::tuple_find<std::tuple<typename TNamedAttribs::tName...>, typename TNamedAttribs::tName>::value]), true));
         }
         vao.draw_elements();
      }

      /// @brief draw arrays
      template<typename...TName, typename...TData>
      void drawArrays(const vao<ct::named_type<TName, TData>...>& vao, GLsizei size, GLenum mode) const
      {
         using tVAOTuple = std::tuple<ct::named_type<TName, TData>...>;
         using tRequiredVAOTuple =  std::tuple<ct::named_type<typename TNamedAttribs::tName,
                                                              typename TNamedAttribs::tAttribTraits::data>...>;
         static_assert(!ct::tuple_any_of<tRequiredVAOTuple, doesnt_contain, tVAOTuple>::value, "not all or not matching type inputs for program was provided by given vao");

         vao.bind();
         // if was attached to different program or wasn't attached at all
         if (mProgramID != vao.mProgramID)
         {
            Log::msg("attaching vao to a program id ", mProgramID);
            vao.mProgramID = mProgramID;
            swallow(vao.template bind_buffer<typename TNamedAttribs::tName>() &&
                    (TNamedAttribs::tAttribTraits::attach(mLocations[ct::tuple_find<std::tuple<typename TNamedAttribs::tName...>, typename TNamedAttribs::tName>::value]), true));
         }
         gl(glDrawArrays, mode, 0, size);
      }

  private:
      /// @brief locations for all VBOs inside vao
      using tLocations = std::array<GLint, sizeof...(TNamedAttribs)>;
      tLocations mLocations;

      /// @brief program id to check in runtime if input vao is ready to be
      /// bound for this program or it should be reattached for this program
      GLuint mProgramID;
};

#endif
