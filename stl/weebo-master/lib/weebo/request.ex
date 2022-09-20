defmodule Weebo.Request do
  @moduledoc "Struct used to represent an XML-RPC request."
  defstruct method: nil, params: []

  @type t :: %Weebo.Request{method: String.t, params: list}
end

defimpl Weebo.Formattable, for: Weebo.Request do
  @spec format(Weebo.Request.t) :: Weebo.xml_tree
  def format(%Weebo.Request{method: name, params: params}) do
    formatted_params = Enum.map params, fn(param) ->
      {:param, [{:value, [Weebo.Formattable.format(param)]}]}
    end
    {:methodCall, [{:methodName, [name]}, {:params, formatted_params}]}
  end
end
