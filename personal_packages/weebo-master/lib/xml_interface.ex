defmodule Weebo.XMLInterface do
  @moduledoc false

  @type xml_element :: {:xmlElement,any,any,any,any,any,any,any,maybe_improper_list(),any,any,any}
  @type xml_text :: {:xmlText,any,any,any,any,any,any,any,maybe_improper_list(),any,any,any}

  require Record
  Record.defrecord :xmlElement, Record.extract(:xmlElement, from_lib: "xmerl/include/xmerl.hrl")
  Record.defrecord :xmlAttribute, Record.extract(:xmlAttribute, from_lib: "xmerl/include/xmerl.hrl")
  Record.defrecord :xmlText, Record.extract(:xmlText, from_lib: "xmerl/include/xmerl.hrl")

  @spec parse(String.t, Keyword.t) :: xml_element
  def parse(xml_string, options \\ [quiet: true]) when is_binary(xml_string) do
    {doc, []} = xml_string
                |> :binary.bin_to_list
                |> :xmerl_scan.string(options)
    doc
  end

  @spec export(xml_element) :: String.t
  def export(xml) do
    List.flatten(:xmerl.export([xml], :xmerl_xml)) |> List.to_string
  end

  @spec from_tree(Weebo.xml_tree) :: xml_element
  def from_tree({elem, children}) do
    content = Enum.map children, fn(child) ->
      if is_bitstring(child) do
        xmlText(value: child)
      else
        from_tree(child)
      end
    end
    xmlElement(name: elem, content: content)
  end

  @spec to_tree(xml_element) :: Weebo.xml_tree
  def to_tree(xml) do
    node = {element_name(xml), []}
    to_tree(element_value(xml), node)
  end

  @spec to_tree(String.t, Weebo.xml_tree) :: Weebo.xml_tree
  def to_tree(string, {name, children}) when is_binary(string) do
    to_tree([], {name, children ++ [string]})
  end

  @spec to_tree(list, Weebo.xml_tree) :: Weebo.xml_tree
  def to_tree([], tree), do: tree
  def to_tree([head|tail], {name, children}) do
    new_node = cond do
      Record.is_record(head, :xmlText) ->
        element_value(head)
      true ->
        to_tree(element_value(head), {element_name(head), []})
    end

    cond do
      is_binary(new_node) and String.strip(new_node)|>String.length == 0 ->
        to_tree(tail, {name, children})
      true ->
        to_tree(tail, {name, children ++ [new_node]})
    end
  end

  @spec element_name(xml_element) :: atom
  def element_name(xmlElement(name: name)), do: name

  @spec element_value(xml_element) :: String.t | list
  def element_value(xmlElement(content: [xmlText(value: value)])), do: to_string(value)
  def element_value(xmlElement(content: list)) when is_list(list), do: list

  @spec element_value(xml_text) :: String.t
  def element_value(xmlText(value: value)), do: to_string(value)
end
