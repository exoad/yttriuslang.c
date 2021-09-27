defprotocol Weebo.Formattable do
  @moduledoc ~S"""
  Protocol that handles trasforming Elixir values into their XML-RPC
  representations.
  """

  @fallback_to_any true

  @doc ~S"""
  Returns a tuple-representation of XML that represents the value passed in.

  ## Example

      defimpl Weebo.Formattable, for: Integer do
        def format(int) do
          {:int, [Integer.to_string(int)]}
        end
      end

  """
  @spec format(any) :: Weebo.xml_tree
  def format(any)
end

defimpl Weebo.Formattable, for: BitString do
  @spec format(String.t) :: Weebo.xml_tree
  def format(string), do: {:string, [string]}
end

defimpl Weebo.Formattable, for: Integer do
  @spec format(integer) :: Weebo.xml_tree
  def format(int), do: {:int, [Integer.to_string(int)]}
end

defimpl Weebo.Formattable, for: Float do
  @spec format(float) :: Weebo.xml_tree
  def format(float), do: {:double, ["#{float}"]}
end

defimpl Weebo.Formattable, for: List do
  @spec format(list) :: Weebo.xml_tree
  def format(list) do
    values = Enum.map list, fn(item) -> {:value, [Weebo.Formattable.format(item)]} end
    {:array, [{:data, values}]}
  end
end

defimpl Weebo.Formattable, for: Map do
  @spec format(map) :: Weebo.xml_tree
  def format(map) do
    entries = Enum.map map, fn({k, v}) ->
      {:member, [{:name, ["#{k}"]}, {:value, [Weebo.Formattable.format(v)]}]}
    end
    {:struct, entries}
  end
end

defimpl Weebo.Formattable, for: Any do
  @spec format(nil) :: Weebo.xml_tree
  def format(nil), do: {:nil, []}

  @spec format(boolean) :: Weebo.xml_tree
  def format(true), do: {:boolean, ["1"]}
  def format(false), do: {:boolean, ["0"]}

  @spec format({{integer, integer, integer}, {integer, integer, integer}}) :: Weebo.xml_tree
  def format({{_,_,_}, {_,_,_}} = time), do: {:"dateTime.iso8601", [:iso8601.format(time)]}
end
