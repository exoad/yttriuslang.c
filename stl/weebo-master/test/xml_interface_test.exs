defmodule Weebo.XMLInterfaceTest do
  use ExUnit.Case
  alias Weebo.XMLInterface, as: XML

  def sample_tree() do
    {:methodResponse, [
          {:params, [
                {:param, [
                      {:value, [
                            {:string, ["Weebo"]},
                          ]}]},
                {:param, [
                      {:value, [
                            {:int, ["0"]}
                          ]}
                    ]}]}]}
  end

  def sample_xml() do
    """
    <?xml version="1.0"?>
    <methodResponse>
      <params>
        <param>
          <value><string>Weebo</string></value>
        </param>
        <param>
          <value><int>0</int></value>
        </param>
      </params>
    </methodResponse>
    """
  end

  def sample_element() do
    "<string>Weebo</string>"
  end

  def sample_empty_element() do
    "<string></string>"
  end

  def strip_xml(xml) do
    String.replace(xml, ~r/[\n\s]*/, "", global: true)
  end

  test "#export" do
    assert XML.parse(sample_xml)|>XML.export|>strip_xml == strip_xml(sample_xml)
  end

  test "#element_name" do
    assert XML.parse(sample_element)|>XML.element_name == :string
  end

  test "#element_value" do
    assert XML.parse(sample_element)|>XML.element_value == "Weebo"
    assert XML.parse(sample_empty_element)|>XML.element_value == []
    assert XML.parse(sample_xml)|>XML.element_value|>is_list
  end

  test "#parse" do
    assert XML.parse(sample_xml)|>is_tuple
  end

  test "#to_tree" do
    assert XML.parse(sample_xml)|>XML.to_tree == sample_tree
  end

  test "#from_tree" do
    XML.from_tree(sample_tree)|>XML.export|>strip_xml == strip_xml(sample_xml)
  end
end
