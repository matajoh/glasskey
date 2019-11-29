module GlassKey
    using CxxWrap
    @wrapmodule(_jlglasskey)

    function __init__()
        @initcxx
    end
end
