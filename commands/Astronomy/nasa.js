const { MessageEmbed } = require('discord.js');
const config = require('../../configs/token.json');
const chnl = require('../../configs/chnl.json');
const resource = require('../../configs/resource.json');
const superagent = require('superagent');
module.exports = {
    config: {
        name: `apod`,
        category: '',
        description: '',
        aliases: [`nasadaily`]
    },
    run: async (bot, message, args) => {
      try {
        const { body } = await superagent.get(`https://api.nasa.gov/planetary/apod?api_key=6xN4Xyh4nluXEufxFnT0fLzku2lEgBUYbv3Orttc`);
  
        if(body.hdurl == undefined){
            const embed = new MessageEmbed()
            .setTitle(`${body.title}`)
            .setDescription(`${body.explanation}`)
            .setThumbnail(resource.nasa)
            .setFooter(`${body.date} | Copyright ${body.copyright}`)
  
            message.channel.send({ embed })
        } else if(body.copyright == undefined) {
            const embed = new MessageEmbed()
            .setTitle(`${body.title}`)
            .setDescription(`${body.explanation}`)
            .setThumbnail(resource.nasa)
            .setImage(`${body.hdurl}`)
            .setFooter(`${body.date}`)
  
            message.channel.send({ embed })
        } else {
            const embed = new MessageEmbed()
            .setTitle(`${body.title}`)
            .setDescription(`${body.explanation}`)
            .setThumbnail(resource.nasa)
            .setImage(`${body.hdurl}`)
            .setFooter(`${body.date} | Copyright ${body.copyright}`)
  
            message.channel.send({ embed })
        }
    } catch (err) {
        const embed = new MessageEmbed()
        .setTitle("Whoops, looks like the requested command doesn't exist (yet)!")
        .setThumbnail(resource.aw_snap)
        .setDescription("Use `"+config.prefix+"help` for a list of avaliable commands!")
        .setFooter("Still facing issues? Join the support server!")
          message.channel.send({ embed }).then(m => {
              m.delete({ timeout: 5000 })
          });
    }
  }
}