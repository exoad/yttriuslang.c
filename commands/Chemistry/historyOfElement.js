const { MessageEmbed } = require('discord.js');
const config = require('../../configs/config.json');
const chnl = require('../../configs/chnl.json');
const resource = require('../../configs/resource.json');
const superagent = require('superagent');
const colors = require('../../configs/colors.json');
var elementHistories = require('element-histories');
module.exports = {
    config: {
        name: `historyelement`,
        category: '',
        description: '',
        aliases: [`elementhistory`, `elehistory`]
    },
    run: async (bot, message, args) => {
    try {
        let toElement = args[0];
        if(!toElement || toElement == undefined) {
            const embed = new MessageEmbed()
            .setTitle("Oops, looks like you are missing some arguments")
            .setDescription("**Usage:** `"+config.prefix+"elehistory {element_name}`\n")
            .setFooter("Have questions? Join the support server!")
            .setThumbnail(resource.aw_snap)
            .setColor(colors.error)
            
            message.channel.send({embed})
        } else {
            const embed = new MessageEmbed()
            .setTitle("Element: "+toElement)
            .addField("Story", elementHistories.story(toElement))
            .addField("Country of Origin", elementHistories.country(toElement))
            .setThumbnail(elementHistories.image(toElement))

            message.channel.send({embed})
        }
    } catch (err) {
        bot.channels.cache.get(chnl.errors).send(`${err.stack}`)
        const embed = new MessageEmbed()
        .setTitle("Hmmm, looks like that element doesn't exist (yet)")
        .setDescription("**Usage:** `"+config.prefix+"element {element_name}`\n")
        .addField("Example", "`"+config.prefix+"elehistory tin`")
        .setFooter("Have questions? Join the support server!")
        .setThumbnail(resource.aw_snap)
        .setColor(colors.error)


        message.channel.send({embed}).then(m => {
            m.delete({ timeout: 10000 })
        })
    }
  }
  }