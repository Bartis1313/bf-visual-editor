#pragma once

#include <cstddef>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

// PS/VS view swap at bind time: reaches every mesh however it cached the view
namespace editor::textures::subst
{
    struct Pair
    {
        ID3D11ShaderResourceView* from;
        ID3D11ShaderResourceView* to;
    };

    void init(ID3D11Device* device, ID3D11DeviceContext* context);
    // count 0 clears; a replaced 'to' must stay alive a few frames (deferred-context binds)
    void set(const Pair* pairs, size_t count);
}
